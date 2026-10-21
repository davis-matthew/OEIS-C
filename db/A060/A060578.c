/*
 * A060578  Number of homeomorphically irreducible general graphs on 3 labeled
 *          node and with n edges.
 * https://oeis.org/A060578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1/120 * Σ_{k=0}^{9} coeff[k] * (n - k + 5 choose 5) ,
 *            where coeff = [1, -3, 6, -8, 24, -51, 70, -66, 36, -8] (OEIS
 *            A060578)
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int coeff[10] = {1, -3, 6, -8, 24, -51, 70, -66, 36, -8};

/* compute binomial(m,5) for m>=5, 0 otherwise.
   Uses 128‑bit integer for safety. */
static inline seq_t binom5(idx_t m)
{
    if (m < 5) return 0;
    unsigned __int128 t = (unsigned __int128)m * (m - 1) * (m - 2) * (m - 3) * (m - 4);
    t /= 120;  /* 5! = 120 */
    return (seq_t)t;
}

seq_t a(idx_t n)
{
    signed __int128 total = 0;
    for (int k = 0; k < 10; ++k) {
        if ((idx_t)k > n) break;
        idx_t m = n - k + 5;
        seq_t bin = binom5(m);
        if (bin != 0) {
            signed __int128 contr = (signed __int128)coeff[k] * (signed __int128)bin;
            total += contr;
        }
    }
    return (seq_t)total;
}
