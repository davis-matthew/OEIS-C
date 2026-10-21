/*
 * A204567  Number of (n+1)X4 0..3 arrays with every 2X2 subblock having equal
 *          diagonal elements or equal antidiagonal elements, and new values
 *          0..3 introduced in row major order.
 * https://oeis.org/A204567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A204567: Number of (n+1)×4 0..3 arrays with every 2×2 subblock having
   equal diagonal elements or equal antidiagonal elements, with new values
   introduced in row‑major order.
   Empirical recurrence (order 7):
   a(n) = 41*a(n‑1) - 451*a(n‑2) + 2007*a(n‑3) - 4185*a(n‑4)
          + 4371*a(n‑5) - 2214*a(n‑6) + 432*a(n‑7)
   Initial values:
   a(1)=256, a(2)=6112, a(3)=158981, a(4)=4220392,
   a(5)=112592256, a(6)=3007468553, a(7)=80358249724 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* First seven terms – stored in an array of type seq_t.  The literals
       are written in the 64‑bit form so that they are promoted to seq_t
       correctly when the target type is wider.  If the target type is
       smaller than 64 bits the constants will be truncated; in that case
       the function is only meaningful for n such that a(n) fits the type. */
    static const seq_t init[7] = {
        256ULL,          /* a(1) */
        6112ULL,         /* a(2) */
        158981ULL,       /* a(3) */
        4220392ULL,      /* a(4) */
        112592256ULL,    /* a(5) */
        3007468553ULL,   /* a(6) */
        80358249724ULL   /* a(7) */
    };

    if (n <= 7)
        return init[n - 1];

    seq_t a0 = init[0]; /* a(n-7) */
    seq_t a1 = init[1]; /* a(n-6) */
    seq_t a2 = init[2]; /* a(n-5) */
    seq_t a3 = init[3]; /* a(n-4) */
    seq_t a4 = init[4]; /* a(n-3) */
    seq_t a5 = init[5]; /* a(n-2) */
    seq_t a6 = init[6]; /* a(n-1) */

    for (idx_t i = 8; i <= n; ++i)
    {
        /* Compute a(i) from the recurrence */
        seq_t cur = 41 * a6
                  - 451 * a5
                  + 2007 * a4
                  - 4185 * a3
                  + 4371 * a2
                  - 2214 * a1
                  + 432  * a0;

        /* Shift the window for the next iteration */
        a0 = a1; a1 = a2; a2 = a3; a3 = a4;
        a4 = a5; a5 = a6; a6 = cur;
    }

    return a6;
}
