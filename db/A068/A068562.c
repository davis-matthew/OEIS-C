/*
 * A068562  Denominators of coefficients in (1+x)^(1/3)-(1-x)^(1/3) power
 *          series.
 * https://oeis.org/A068562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = 3^{ (2n-1) + v_3((2n-1)!) } where v_3(m!) = Σ_{i≥1}
 *            ⌊m/3^i⌋
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow3(idx_t e)
{
    seq_t result = 1;
    seq_t base   = 3;
    while (e > 0)
    {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; values for n<1 are undefined */
    idx_t k = 2 * n - 1;           /* odd index in the binomial series */
    idx_t e = k;                   /* start with k */
    idx_t t = k;
    while (t) {                    /* add v_3(k!) */
        t /= 3;
        e += t;
    }
    return pow3(e);
}
