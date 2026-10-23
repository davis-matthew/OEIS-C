/*
 * A121525  Number of up steps starting at an odd level in all nondecreasing
 *          Dyck paths of semilength n. A nondecreasing Dyck path is a Dyck
 *          path for which the sequence of the altitudes of the valleys is
 *          nondecreasing.
 * https://oeis.org/A121525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A121525: Number of up steps starting at an odd level in all
   nondecreasing Dyck paths of semilength n.
   The sequence satisfies the linear recurrence
       a(n) = 6*a(n-1) - 9*a(n-2) -5*a(n-3) +15*a(n-4)
              - a(n-5) -4*a(n-6) + a(n-7)
   for n ≥ 8, with initial values
       a(1)=0, a(2)=1, a(3)=5, a(4)=19, a(5)=67,
       a(6)=219, a(7)=690.  */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return directly for the first seven terms. */
    if (n <= 7)
    {
        static const seq_t init[7] = {0, 1, 5, 19, 67, 219, 690};
        return init[n - 1];
    }

    /* Use a sliding window of the last seven values. */
    seq_t v[7];
    v[0] = 0;   /* a(1)  */
    v[1] = 1;   /* a(2)  */
    v[2] = 5;   /* a(3)  */
    v[3] = 19;  /* a(4)  */
    v[4] = 67;  /* a(5)  */
    v[5] = 219; /* a(6)  */
    v[6] = 690; /* a(7)  */

    for (idx_t i = 8; i <= n; ++i)
    {
        /* Compute a(i) from the previous seven terms. */
        seq_t cur = 6 * v[6]
                  - 9 * v[5]
                  - 5 * v[4]
                  +15 * v[3]
                  -   v[2]
                  - 4 * v[1]
                  +   v[0];

        /* Shift the window: drop a(i-7), move others forward. */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = v[6];
        v[6] = cur;          /* now v[6] holds a(i)   */
    }

    return v[6];
}
