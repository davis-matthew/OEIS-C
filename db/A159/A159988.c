/*
 * A159988  Catalan numbers read modulo 11 .
 * https://oeis.org/A159988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Catalan(n) mod 11 using Lucas theorem
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   powmod11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/powmod11.h"
/* Pre‑compute binomial coefficients C(n,k) mod 11 for 0≤n,k≤10 */
static int binom_small[11][11];
static int init_lucas = 0;

/* initialise the small binomial table */
static void init_lucas_table(void)
{
    int fact[11];
    fact[0] = 1;
    for (int i = 1; i <= 10; ++i) fact[i] = fact[i - 1] * i % 11;

    int invfact[11];
    for (int i = 0; i <= 10; ++i) invfact[i] = powmod11(fact[i], 9);  /* 11-2 */

    for (int n = 0; n <= 10; ++n)
    {
        for (int k = 0; k <= 10; ++k)
        {
            if (k > n) binom_small[n][k] = 0;
            else
            {
                int v = fact[n];
                v = v * invfact[k] % 11;
                v = v * invfact[n - k] % 11;
                binom_small[n][k] = v;
            }
        }
    }
    init_lucas = 1;
}

/* Lucas theorem for binomial coefficient modulo 11 */
static int lucas_binom(idx_t N, idx_t K)
{
    if (!init_lucas) init_lucas_table();
    if (K > N) return 0;

    int res = 1;
    while (N > 0 || K > 0)
    {
        int ni = N % 11;
        int ki = K % 11;
        if (ki > ni) return 0;
        res = res * binom_small[ni][ki] % 11;
        N /= 11;
        K /= 11;
    }
    return res;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (!init_lucas) init_lucas_table();

    idx_t two_n = n + n;
    int binon = lucas_binom(two_n, n);          /* C(2n, n)  */
    int binop = lucas_binom(two_n, n + (idx_t)1);/* C(2n, n+1)*/

    int r = binon - binop;
    if (r < 0) r += 11;        /* ensure non‑negative */

    return (seq_t)r;
}
