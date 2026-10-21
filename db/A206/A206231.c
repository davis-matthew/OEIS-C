/*
 * A206231  Number of (n+1) X 2 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two equal edges, and new values 0..3 introduced in
 *          row major order.
 * https://oeis.org/A206231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence
 *            a(n)=8*a(n-1)-11*a(n-2)+36*a(n-3)-303*a(n-4)+232*a(n-5)+147*a(n-6)+756*a(n-7)
 *            for n>8. Initial values: a(1)=15, a(2)=60, a(3)=310, a(4)=1640,
 *            a(5)=8910, a(6)=51066, a(7)=294546, a(8)=1710184
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return zero for non‑positive indices – not part of the
       OEIS offset but harmless. */
    if (n == 0) return 0;

    /* Initial 8 values (index 1 to 8).  Use explicit cast to seq_t
       in case seq_t is a class type. */
    static const seq_t init[8] =
        { (seq_t)15,  (seq_t)60,  (seq_t)310,  (seq_t)1640,
          (seq_t)8910, (seq_t)51066, (seq_t)294546, (seq_t)1710184 };

    /* For n up to 8 we can return the pre‑computed value directly. */
    if (n <= 8) return init[n-1];

    /* Buffer holding the last seven values needed for the recurrence.
       buf[0] = a(k-1), buf[1] = a(k-2), ..., buf[6] = a(k-7). */
    seq_t buf[7];

    /* Initialise the buffer for k = 9 (i.e. a(8) … a(2)). */
    buf[0] = init[7];          /* a(8) */
    for (idx_t i = 1; i < 7; ++i) {
        buf[i] = init[7-i];    /* a(7), a(6), …, a(2) */
    }

    seq_t val = 0;              /* will hold a(k) each iteration */

    /* Compute terms from a(9) up to a(n). */
    for (idx_t k = 9; k <= n; ++k) {
        val = 8*buf[0] - 11*buf[1] + 36*buf[2] - 303*buf[3]
            + 232*buf[4] + 147*buf[5] + 756*buf[6];

        /* Shift buffer: drop the oldest value (a(k-7)) and
           prepend the newly computed a(k). */
        for (idx_t i = 6; i > 0; --i) buf[i] = buf[i-1];
        buf[0] = val;
    }

    return val;       /* a(n) */
}
