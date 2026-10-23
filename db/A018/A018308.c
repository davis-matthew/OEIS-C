/*
 * A018308  Divisors of 154.
 * https://oeis.org/A018308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n‑th divisor of 154 in ascending order (offset 1)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* comparison helper for qsort */
static int cmp_seqt(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* static storage for the eight divisors */
static seq_t divisors[8];
static bool initialized = false;

/* initialise the divisor table */
static void init_divisors(void)
{
    /* prime factorisation of 154 : 2 * 7 * 11 */
    const seq_t primes[3] = {(seq_t)2, (seq_t)7, (seq_t)11};
    size_t idx = 0;
    for (size_t mask = 0; mask < (1ULL << 3); ++mask) {
        seq_t prod = (seq_t)1;
        for (size_t bit = 0; bit < 3; ++bit) {
            if (mask & (1ULL << bit))
                prod = prod * primes[bit];
        }
        divisors[idx++] = prod;
    }
    /* sort the divisors into ascending order */
    qsort(divisors, 8, sizeof(seq_t), cmp_seqt);
}

/* sequence function */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_divisors();
        initialized = true;
    }
    if (n == 0) return (seq_t)0;
    if (n <= 8) return divisors[n - 1];
    return (seq_t)0;  /* undefined beyond the finite set */
}
