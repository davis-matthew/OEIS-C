/*
 * A236812  Number of (n+1)X(1+1) 0..2 arrays with the maximum plus the lower
 *          median minus the minimum of every 2X2 subblock equal.
 * https://oeis.org/A236812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) satisfies the empirical linear recurrence *
 *            a(n)=9*a(n-1)-2*a(n-2)-181*a(n-3)+349*a(n-4)+1204*a(n-5)-3474*a(n-6)
 *            *
 *            -2856*a(n-7)+13672*a(n-8)-432*a(n-9)-24464*a(n-10)+10352*a(n-11)
 *            * +18944*a(n-12)-11968*a(n-13)-4736*a(n-14)+3584*a(n-15) * with
 *            initial values * a(1)=81, a(2)=303, a(3)=1162,...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices < 1 as the sequence is defined for n >= 1. */
    if (n < 1) return 0;

    /* Initial 15 terms of the sequence. */
    static const seq_t init[15] = {
        (seq_t)81,          /* a(1) */
        (seq_t)303,         /* a(2) */
        (seq_t)1162,        /* a(3) */
        (seq_t)4627,        /* a(4) */
        (seq_t)18527,       /* a(5) */
        (seq_t)75330,       /* a(6) */
        (seq_t)307321,      /* a(7) */
        (seq_t)1264403,     /* a(8) */
        (seq_t)5216986,     /* a(9) */
        (seq_t)21640527,    /* a(10) */
        (seq_t)89986987,    /* a(11) */
        (seq_t)375549402,   /* a(12) */
        (seq_t)1570552173,  /* a(13) */
        (seq_t)6585201767,  /* a(14) */
        (seq_t)27658875370  /* a(15) */
    };

    /* If the requested index is within the pre‑computed range,
       return the stored value directly. */
    if (n <= 15)
        return init[n - 1];

    /* Circular buffer holding the last 15 computed terms,
       initially set to the initial values. */
    seq_t buf[15];
    for (int i = 0; i < 15; ++i)
        buf[i] = init[i];

    /* Main recurrence loop: compute a(16) … a(n). */
    for (idx_t k = 16; k <= n; ++k) {
        /* Compute the next term using the given recurrence. */
        seq_t next =
            (seq_t)9      * buf[14]  - (seq_t)2      * buf[13] -
            (seq_t)181    * buf[12]  + (seq_t)349    * buf[11] +
            (seq_t)1204   * buf[10]  - (seq_t)3474   * buf[9]  -
            (seq_t)2856   * buf[8]   + (seq_t)13672  * buf[7]  -
            (seq_t)432    * buf[6]   - (seq_t)24464   * buf[5]  +
            (seq_t)10352  * buf[4]   + (seq_t)18944   * buf[3]  -
            (seq_t)11968  * buf[2]   - (seq_t)4736    * buf[1]  +
            (seq_t)3584   * buf[0];

        /* Shift the buffer left by one element and place the new term at the end. */
        for (int i = 0; i < 14; ++i)
            buf[i] = buf[i + 1];
        buf[14] = next;
    }

    /* After the loop, buf[14] holds a(n). */
    return buf[14];
}
