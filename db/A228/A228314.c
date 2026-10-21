/*
 * A228314  Triangle read by rows: T(p,q) (1<=q<=p) is the hyper-Wiener index
 *          of the Cartesian product of the cycles C(p) and C(q) (Torus Grid
 *          Graph).
 * https://oeis.org/A228314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A228314 – triangle read by rows. For each n ≥ 1 we recover the
 *            position (p,q) (1 ≤ q ≤ p) in the upper-triangular array and
 *            compute T(p,q) by the four parity‑dependent formulas given in
 *            the OEIS entry
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find p such that n ≤ p*(p+1)/2 */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 cum = (unsigned __int128)mid * (mid + 1) / 2;
        if (cum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t p = low;

    /* Compute q = n – (p−1)p/2 */
    unsigned __int128 prev = (unsigned __int128)(p - 1) * p / 2;
    idx_t q = n - (idx_t)prev;               /* 1 ≤ q ≤ p */

    seq_t pp = (seq_t)p;
    seq_t qq = (seq_t)q;
    seq_t result = 0;

    int p_even = (p & 1) == 0;
    int q_even = (q & 1) == 0;

    if (p_even && q_even) {                    /* both even */
        seq_t term = (seq_t)8
                    + (seq_t)6 * pp
                    + (seq_t)6 * qq
                    + (seq_t)2 * pp * pp
                    + (seq_t)3 * pp * qq
                    + (seq_t)2 * qq * qq;
        seq_t val = pp * pp * qq * qq * term;
        result = val / (seq_t)96;
    } else if (!p_even && !q_even) {            /* both odd */
        seq_t term = (seq_t)3
                      - (seq_t)4 * pp * qq
                      - (seq_t)6 * pp
                      - (seq_t)6 * qq
                      - (seq_t)3 * pp * pp
                      - (seq_t)3 * qq * qq
                      + (seq_t)6 * pp * qq * qq
                      + (seq_t)6 * pp * pp * qq
                      + (seq_t)2 * pp * qq * qq * qq
                      + (seq_t)3 * pp * pp * qq * qq
                      + (seq_t)2 * pp * pp * pp * qq;
        seq_t val = pp * qq * term;
        result = val / (seq_t)96;
    } else if (!p_even && q_even) {             /* p odd, q even */
        seq_t term = (seq_t)2 * pp
                     - (seq_t)3 * qq
                     - (seq_t)6
                     + (seq_t)6 * pp * qq
                     + (seq_t)6 * pp * pp
                     + (seq_t)2 * pp * qq * qq
                     + (seq_t)3 * pp * pp * qq
                     + (seq_t)2 * pp * pp * pp;
        seq_t val = pp * qq * qq * term;
        result = val / (seq_t)96;
    } else { /* p even, q odd */
        seq_t term = (seq_t)2 * qq
                     - (seq_t)3 * pp
                     - (seq_t)6
                     + (seq_t)6 * pp * qq
                     + (seq_t)6 * qq * qq
                     + (seq_t)2 * qq * pp * pp
                     + (seq_t)3 * pp * qq * qq
                     + (seq_t)2 * qq * qq * qq;
        seq_t val = pp * pp * qq * term;
        result = val / (seq_t)96;
    }

    return result;
}
