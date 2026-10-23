/*
 * A035777  Coordination sequence for C_40 lattice.
 * https://oeis.org/A035777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^40
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 35,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

/* Pre‑compute the 41 binomial coefficients C(40,i). */
static seq_t binom40[41];
static bool binom40_inited = false;

static void init_binom40(void)
{
    binom40[0] = 1;
    for (idx_t i = 1; i <= 40; ++i)
    {
        binom40[i] = binom40[i-1] * (seq_t)(40 - i + 1) / (seq_t)i;
    }
    binom40_inited = true;
}

/* Compute binomial coefficient C(n,k) for 0 <= k <= n.
   Uses successive division with GCD to avoid overflow in intermediate
   products.  This routine works for any seq_t that supports + - * / and
   the gcd(seq_t,seq_t) routine from utils/gcd.h. */
static seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = res * (seq_t)(n - k + i);
        seq_t den = (seq_t)i;
        seq_t g   = gcd(num, den);
        num /= g;
        den /= g;
        res = num / den;   /* guaranteed to divide exactly */
    }
    return res;
}

/* Main sequence function.  It may be called in any order.
   The computation is O(40) per call. */
seq_t a(idx_t n)
{
    if (!binom40_inited)
        init_binom40();

    idx_t m = n << 1;                    /* m = 2*n */
    idx_t i_max = (m < 40) ? m : 40;     /* upper bound for i */
    seq_t sum = 0;

    for (idx_t i = 0; i <= i_max; ++i)
    {
        seq_t c1 = binom40[i];
        seq_t c2 = binom_seq(39 + (m - i), 39);
        sum += c1 * c2;
    }
    return sum;
}
