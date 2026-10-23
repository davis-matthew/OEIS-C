/*
 * A117977  Legendre-binomial transform of 3^n for p=3.
 * https://oeis.org/A117977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..n} L(C(n,k)/3) * 3^k, where * L(j/3) is the
 *            Legendre symbol modulo 3 of j
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper table: binomial coefficient modulo 3 for digits 0..2. */
static const int binom_mod3[3][3] = {
    /* k=0 1 2 */
    {1, 0, 0}, /* n=0 */
    {1, 1, 0}, /* n=1 */
    {1, 2, 1}  /* n=2 */
};

/* Determine Legendre symbol of C(n,k) modulo 3.
   Returns 0 if C(n,k) is divisible by 3, +1 or -1 otherwise. */
static int legendre_mod3(idx_t n, idx_t k)
{
    int sign = 1;
    while (n || k) {
        int nd = n % 3;
        int kd = k % 3;
        int val = binom_mod3[nd][kd];
        if (val == 0)   /* k_i > n_i  -> C(n,k) ≡ 0 (mod 3) */
            return 0;
        if (val == 2)   /* 2 ≡ -1 (mod 3) */
            sign = -sign;
        n /= 3;
        k /= 3;
    }
    return sign;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    seq_t pos = 0;
    seq_t neg = 0;
    seq_t pow3 = 1;        /* 3^k for current k */

    for (idx_t k = 0; k <= n; ++k) {
        int s = legendre_mod3(n, k);
        if (s > 0)
            pos += pow3;
        else if (s < 0)
            neg += pow3;
        /* prepare next power of 3 */
        pow3 *= (seq_t)3;
    }

    /* pos >= neg for all n (sequence values are positive). */
    seq_t result = pos;
    result -= neg;
    return result;
}
