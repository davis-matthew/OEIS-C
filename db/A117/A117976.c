/*
 * A117976  Legendre-binomial transform of 2^n for p=3.
 * https://oeis.org/A117976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum{k=0..n} Legendre_3( C(n,k) ) * 2^k * where
 *            Legendre_3(j) = 0 if j ≡ 0 (mod 3), 1 if j ≡ 1 (mod 3), * and -1
 *            if j ≡ 2 (mod 3). The Legendre symbol modulo the prime * p = 3
 *            is used below. The implementation uses Lucas' theorem * to
 *            compute C(n,k) modulo 3 efficiently
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*=====================================================================*/
#include <stdint.h>

/* Small binomial table for digits 0..2 inclusive modulo 3 */
static const int _C_mod3[3][3] = {
    {1, 0, 0},        /* 0C0 = 1, 0C1 = 0, 0C2 = 0   */
    {1, 1, 0},        /* 1C0 = 1, 1C1 = 1, 1C2 = 0   */
    {1, 2, 1}         /* 2C0 = 1, 2C1 = 2, 2C2 = 1   */
};

/* Compute C(n,k) modulo 3 using Lucas theorem (prime 3). */
static int _binom_mod3(idx_t n, idx_t k)
{
    int res = 1;
    while (n > 0 || k > 0) {
        int ni = n % 3;
        int ki = k % 3;
        if (ki > ni) return 0;
        int ck = _C_mod3[ni][ki];
        res = (res * ck) % 3;
        n /= 3;
        k /= 3;
    }
    return res;
}

/*=====================================================================*/
seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t sum   = 0;
    seq_t power = 1;               /* 2^0 */
    for (idx_t k = 0; k <= n; ++k) {
        int mod = _binom_mod3(n, k);     /* 0,1,2 */
        if (mod == 1) {
            sum += power;
        } else if (mod == 2) {
            sum -= power;
        }
        power *= (seq_t)2;   /* next power of two */
    }
    return sum;
}
