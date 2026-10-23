/*
 * A018486  Divisors of 495.
 * https://oeis.org/A018486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 495 in ascending order. Since 495 =
 *            3^2 * 5 * 11, there are exactly 12 divisors
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#ifndef SEQ_T
/* seq_t is supplied by the build. */
#endif

/* Static cache of divisors */
static seq_t divs[12] = {0};
static size_t divcnt = 0;
static int init_done = 0;

/* Initialize divisor array once */
static void init_divs(void)
{
    if (init_done) return;
    init_done = 1;

    /* Powers of the prime factors */
    seq_t p3[3], p5[2], p11[2];
    p3[0] = 1; p3[1] = (seq_t)3; p3[2] = (seq_t)9;
    p5[0] = 1; p5[1] = (seq_t)5;
    p11[0] = 1; p11[1] = (seq_t)11;

    size_t idx = 0;
    for (size_t i = 0; i < 3; ++i)          /* exponent of 3 : 0..2 */
        for (size_t j = 0; j < 2; ++j)      /* exponent of 5 : 0..1 */
            for (size_t k = 0; k < 2; ++k)  /* exponent of 11: 0..1 */
            {
                seq_t val = p3[i] * p5[j] * p11[k];
                divs[idx++] = val;
            }
    divcnt = idx;

    /* Simple in‑place sort (insertion sort) */
    for (size_t i = 1; i < divcnt; ++i)
    {
        seq_t temp = divs[i];
        size_t j = i;
        while (j > 0 && divs[j-1] > temp)
        {
            divs[j] = divs[j-1];
            --j;
        }
        divs[j] = temp;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* out of range (should not happen) */
    init_divs();
    if ((idx_t)divcnt < n) return 0;   /* n beyond number of divisors */
    return divs[n-1];
}
