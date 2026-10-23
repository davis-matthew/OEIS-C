/*
 * A182017  Row square-sums of triangle A182013.
 * https://oeis.org/A182017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} ( Sum_{i=k..n} M(i) )^2 where M(i) are
 *            Motzkin numbers (A001006)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for Motzkin numbers */
static seq_t *motz = NULL;
static idx_t motz_len = 0; /* number of elements stored, motz[0..motz_len-1] */

/* Ensure that motz contains Motzkin numbers up to index n (inclusive). */
static void ensure_motz_idx(idx_t n)
{
    if (n + 1 <= motz_len)
        return; /* already computed */

    /* allocate new space */
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(motz, new_cap * sizeof(seq_t));
    if (!tmp) abort();      /* memory allocation failure */
    motz = tmp;

    /* compute any new entries */
    for (idx_t i = motz_len; i <= n; ++i) {
        if (i == 0) {
            motz[0] = (seq_t)1;
        } else if (i == 1) {
            motz[1] = (seq_t)1;
        } else {
            /* Motzkin recurrence: M(i) = M(i-1) + sum_{k=0}^{i-2} M(k)*M(i-2-k) */
            seq_t sum = 0;
            idx_t max_k = i - 2;
            for (idx_t k = 0; k <= max_k; ++k) {
                sum += motz[k] * motz[max_k - k];
            }
            motz[i] = motz[i - 1] + sum;
        }
    }
    motz_len = new_cap;
}

/* Main function to compute a(n) */
seq_t a(idx_t n)
{
    /* build Motzkin numbers up to n */
    ensure_motz_idx(n);

    seq_t sum   = 0;   /* running suffix sum of Motzkin numbers */
    seq_t total = 0;   /* accumulated sum of squares */

    /* iterate k from n down to 0 */
    for (idx_t k = n; ; --k) {
        sum   += motz[k];
        total += sum * sum;
        if (k == 0)
            break;       /* avoid unsigned underflow */
    }
    return total;
}
