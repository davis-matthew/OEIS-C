/*
 * A081095  5th differences of partition numbers A000041.
 * https://oeis.org/A081095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 30, i16 n <= 110, i32 n <= 269, i64 n <= 691,
 *            i128 n <= 2027, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 5th forward difference of partition numbers A000041
 * a(n) = Σ_{j=0}^5 (-1)^(5-j) * C(5,j) * p(n+j)
 * where p(m) is the integer partition function (p(m)=0 for m<0)     */

#include <stdlib.h>

/* Cache of partition numbers p(0) … p(max_n)  */
static seq_t *part_cache   = NULL;
static size_t part_cap = 0;

/* Ensure part_cache holds p(0) … p(max_n) */
static void ensure_partitions(idx_t max_n)
{
    if ((size_t)max_n + 1 <= part_cap)
        return; /* already computed up to the required index */

    /* Grow or allocate the cache */
    size_t new_cap = (size_t)max_n + 1;
    if (part_cache == NULL) {
        part_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        part_cache = (seq_t *)realloc(part_cache, new_cap * sizeof(seq_t));
    }
    if (part_cache == NULL) {
        /* allocation failed – not expected for the test harness */
        exit(EXIT_FAILURE);
    }
    part_cap = new_cap;

    /* Initialize first partition number */
    if (part_cap >= 1) part_cache[0] = 1;

    /* Compute missing partition numbers up to max_n
     * (start after 0 because p(0) is already known) */
    for (idx_t n = 1; n <= max_n; ++n) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > n)
                break;
            seq_t sign = (k & 1) ? 1 : -1;
            sum += sign * part_cache[n - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > n)
                continue;
            sum += sign * part_cache[n - pent2];
        }
        part_cache[n] = sum;
    }
}

seq_t a(idx_t n)
{
    /* Need partition numbers up to n+5 for the forward difference */
    ensure_partitions(n + 5);

    /* Binomial coefficients of (1 + x)^5 */
    const seq_t coeff[6] = {1, 5, 10, 10, 5, 1};

    seq_t result = 0;
    for (int j = 0; j <= 5; ++j) {
        seq_t term = coeff[j] * part_cache[n + j];
        if ((5 - j) & 1)   /* alternate sign */
            term = -term;
        result += term;
    }
    return result;
}
