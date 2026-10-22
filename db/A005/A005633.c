/*
 * A005633  Bishops on an n X n board (see Robinson paper for details).
 * https://oeis.org/A005633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   with t(k) = A000085(k) (involutions: t(0)=t(1)=1,
 *            t(k)=t(k-1)+(k-1)*t(k-2)) and B(j) = A000898(j) (B(0)=1, B(1)=2,
 *            B(j)=2*B(j-1)+(2j-2)*B(j-2)): a(2m) = m!*(m+2)/4 for m even,
 *            (m-1)!*(m+1)^2/4 for m odd; a(2m+1) = (t(m)*t(m+1) -
 *            B(floor(m/2))*B(ceil(m/2)))/2. Halves are carried through the
 *            recurr...
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 24, u64 n <= 38, u128 n <= 64,
 *            big any n
 * Helpers:   b_full, b_half, inv_h, inv_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/b_full.h"
#include "../utils/b_half.h"
#include "../utils/inv_h.h"
#include "../utils/inv_t.h"

seq_t a(idx_t n)
{
    idx_t m = n / 2, i;
    if (n % 2 == 0) {
        seq_t f = 1;
        if (m % 2 == 0) {
            /* (m!/2) * ((m+2)/2) */
            for (i = 3; i <= m; i++) f *= (seq_t)i;
            return f * (seq_t)((m + 2) / 2);
        } else {
            /* (m-1)! * ((m+1)/2)^2 */
            seq_t h = (seq_t)((m + 1) / 2);
            for (i = 2; i + 1 <= m; i++) f *= (seq_t)i;
            return f * h * h;
        }
    }
    if (m == 0) return 0;
    {
        idx_t fl = m / 2, ce = m - m / 2;
        return inv_h(m + 1) * inv_t(m) - b_half(ce) * b_full(fl);
    }
}
