/*
 * A220591  Number of ways to reciprocally link elements of an nX3 array
 *          either to themselves or to exactly two king-move neighbors.
 * https://oeis.org/A220591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A220591: Number of ways to reciprocally link elements of an
    n×3 array either to themselves or to exactly two king‑move neighbors.
    Recurrence (order 10) (offset 1):
        a(n) = 11*a(n-1) + 46*a(n-2) + 14*a(n-3)
               - 46*a(n-4) - 63*a(n-5) + 35*a(n-6)
               + 20*a(n-7) + a(n-8) - 4*a(n-9) - a(n-10)
    Initial values:
        a(1)=1, a(2)=33, a(3)=423, a(4)=6156, a(5)=87542,
        a(6)=1250507, a(7)=17847210, a(8)=254759570,
        a(9)=3636434730, a(10)=51906768670
*/

#include <stdint.h>   /* for ULL suffixes */

/* Store the first 10 terms (index 0 is unused). */
static const seq_t init[11] = {
    0,                   /* dummy index 0 */
    1,                   /* a(1)   */
    33,                  /* a(2)   */
    423,                 /* a(3)   */
    6156,                /* a(4)   */
    87542,               /* a(5)   */
    1250507,             /* a(6)   */
    17847210,            /* a(7)   */
    254759570,           /* a(8)   */
    3636434730ULL,       /* a(9)   */
    51906768670ULL       /* a(10)  */
};

seq_t a(idx_t n)
{
    /* Directly return the initial terms. */
    if (n <= 10) {
        return init[(idx_t)n];
    }

    /* Temporarily hold the ten most recent values.
       After each iteration prev[0] contains a(current),
       prev[1] a(current-1), …, prev[9] a(current-9). */
    seq_t prev[10];

    /* Initialise prev with a(10) … a(1). */
    prev[0] = init[10];
    prev[1] = init[9];
    prev[2] = init[8];
    prev[3] = init[7];
    prev[4] = init[6];
    prev[5] = init[5];
    prev[6] = init[4];
    prev[7] = init[3];
    prev[8] = init[2];
    prev[9] = init[1];

    /* Iteratively compute a(11) … a(n). */
    for (idx_t cur = 11; cur <= n; ++cur) {
        seq_t pos = prev[0] * (seq_t)11
                  + prev[1] * (seq_t)46
                  + prev[2] * (seq_t)14
                  + prev[5] * (seq_t)35
                  + prev[6] * (seq_t)20
                  + prev[7] * (seq_t)1;

        seq_t neg = prev[3] * (seq_t)46
                  + prev[4] * (seq_t)63
                  + prev[8] * (seq_t)4
                  + prev[9] * (seq_t)1;

        seq_t next = pos;
        next -= neg;          /* a(cur) = pos - neg  */

        /* Shift the window so that prev[0] becomes a(cur). */
        for (int k = 9; k >= 1; --k) {
            prev[k] = prev[k - 1];
        }
        prev[0] = next;
    }

    /* After the loop prev[0] holds a(n). */
    return prev[0];
}
