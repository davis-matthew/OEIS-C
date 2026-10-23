/*
 * A194021  Number of ways to arrange 4 points on an n X n X n triangular grid
 *          on an isosceles triangle so that it balances at the midpoint of
 *          its central altitude.
 * https://oeis.org/A194021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 128, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - 2*a(n-2) - 2*a(n-4) + 5*a(n-6) - 3*a(n-7)
 *              + 3*a(n-8) - 5*a(n-9) + 2*a(n-11) + 2*a(n-13) - 3*a(n-14)
 *              + a(n-15)        for n ≥ 16
 *
 * Initial values (1‑based):
 * a(1)=0, a(2)=0, a(3)=1, a(4)=8, a(5)=26, a(6)=75, a(7)=182,
 * a(8)=387, a(9)=756, a(10)=1382, a(11)=2379, a(12)=3915, a(13)=6198,
 * a(14)=9486, a(15)=14110.
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values for 1 ≤ n ≤ 15 (index 0 corresponds to a(1)) */
    static const seq_t base[15] = {
        (seq_t)0,  /* a(1) */
        (seq_t)0,  /* a(2) */
        (seq_t)1,  /* a(3) */
        (seq_t)8,  /* a(4) */
        (seq_t)26, /* a(5) */
        (seq_t)75, /* a(6) */
        (seq_t)182,/* a(7) */
        (seq_t)387,/* a(8) */
        (seq_t)756,/* a(9) */
        (seq_t)1382,/* a(10) */
        (seq_t)2379,/* a(11) */
        (seq_t)3915,/* a(12) */
        (seq_t)6198,/* a(13) */
        (seq_t)9486,/* a(14) */
        (seq_t)14110 /* a(15) */
    };

    /* Return the initial value directly if inside the pre‑computed range */
    if (n <= 15 && n >= 1) {
        return base[n - 1];
    }

    /* Buffer holding the last 15 values:
       buf[0] = a(k), buf[1] = a(k-1), … buf[14] = a(k-14)   */
    seq_t buf[15];

    /* Initialise buffer with a(15) … a(1)  (k = 15 at start) */
    for (int j = 0; j < 15; ++j) {
        buf[j] = base[14 - j];
    }

    /* Compute successive values until the desired n is reached */
    for (idx_t k = 16; k <= n; ++k) {
        /* Positive contributions of the recurrence */
        seq_t pos = (seq_t)3 * buf[0]
                  + (seq_t)5 * buf[5]
                  + (seq_t)3 * buf[7]
                  + (seq_t)2 * buf[10]
                  + (seq_t)2 * buf[12]
                  + buf[14];

        /* Negative contributions of the recurrence */
        seq_t neg = (seq_t)2 * buf[1]
                  + (seq_t)2 * buf[3]
                  + (seq_t)3 * buf[6]
                  + (seq_t)5 * buf[8]
                  + (seq_t)3 * buf[13];

        seq_t next = pos - neg;  /* a(k) satisfies pos ≥ neg for all k */

        /* Shift buffer: new value becomes buf[0] */
        for (int j = 14; j > 0; --j) {
            buf[j] = buf[j - 1];
        }
        buf[0] = next;
    }

    return buf[0]; /* a(n) */
}
