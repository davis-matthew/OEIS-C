/*
 * A178877  Divisors of 1260.
 * https://oeis.org/A178877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 1260 – a(n) is the n-th divisor of 1260 in ascending order */

/* Computes a(n) for the OEIS entry A178877.
   The sequence contains the 36 positive divisors of 1260,
   sorted in increasing order. For n > 36 or n < 1,
   the function returns 0. */
#include <stdlib.h>

static int cmp_seq(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* On first use, generate all 36 divisors of 1260. */
    static seq_t divs[36];
    static size_t div_count = 0;

    if (div_count == 0) {
        /* Pre‑compute prime powers. */
        seq_t p2[3] = {1, 2, 4};          /* 2^0 .. 2^2  */
        seq_t p3[3] = {1, 3, 9};          /* 3^0 .. 3^2  */
        seq_t p5[2] = {1, 5};             /* 5^0 .. 5^1  */
        seq_t p7[2] = {1, 7};             /* 7^0 .. 7^1  */

        size_t idx = 0;
        for (idx_t a = 0; a < 3; ++a) {
            for (idx_t b = 0; b < 3; ++b) {
                for (idx_t c = 0; c < 2; ++c) {
                    for (idx_t d = 0; d < 2; ++d) {
                        divs[idx++] = p2[a] * p3[b] * p5[c] * p7[d];
                    }
                }
            }
        }
        div_count = idx; /* Should be 36. */

        /* Sort the divisors in ascending order. */
        qsort(divs, div_count, sizeof(seq_t), cmp_seq);
    }

    /* Validate index and return the requested divisor. */
    if (n < 1 || n > div_count)
        return (seq_t)0;
    return divs[(size_t)(n - 1)];
}
