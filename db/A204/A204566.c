/*
 * A204566  Number of (n+1) X 3 0..3 arrays with every 2 X 2 subblock having
 *          equal diagonal elements or equal antidiagonal elements, and new
 *          values 0..3 introduced in row major order.
 * https://oeis.org/A204566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 19*a(n‑1) - 79*a(n‑2) + 57*a(n‑3) + 16*a(n‑4) - 12*a(n‑5)
 *            for n ≥ 6, with a(1)=42, a(2)=482, a(3)=6112, a(4)=80518,
 *            a(5)=1075080
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial values (1‑based index) */
    static const seq_t init[6] = { 0,
        42,        /* a(1) */
        482,       /* a(2) */
        6112,      /* a(3) */
        80518,     /* a(4) */
        1075080    /* a(5) */
    };

    /* n is guaranteed ≥1 */
    if (n <= 5)
        return init[n];

    /* compute iteratively using the recurrence */
    seq_t s1 = init[1];
    seq_t s2 = init[2];
    seq_t s3 = init[3];
    seq_t s4 = init[4];
    seq_t s5 = init[5];
    seq_t next;

    /* loop from 6 up to n */
    for (idx_t i = 6; i <= n; ++i)
    {
        /* For unsigned seq_t the intermediate sum is always >= the
           subtracted part, so no underflow occurs. */
        seq_t sum1 = 19 * s5 + 57 * s3 + 16 * s2;
        seq_t sum2 = 79 * s4 + 12 * s1;
        next = sum1 - sum2;

        /* shift the window */
        s1 = s2;
        s2 = s3;
        s3 = s4;
        s4 = s5;
        s5 = next;
    }

    return s5;
}
