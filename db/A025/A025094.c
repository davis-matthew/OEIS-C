/*
 * A025094  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (Lucas numbers), t = A000201 (lower Wythoff sequence).
 * https://oeis.org/A025094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{i=1}^{⌈n/2⌉} L_i * A000201(n-i+1), where L_i is the
 *            i-th Lucas number (L_1=1, L_2=3, L_{i}=L_{i-1}+L_{i-2}), and
 *            A000201(m) is the lower Wythoff sequence, given by A000201(m) =
 *            ⌊(m+1)·φ⌋ with φ = (1+√5)/2
 * Range:     u8 n <= 8, u16 n <= 28, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root for an unsigned 128‑bit integer.
   Returns ⌊√n⌋. */
static idx_t isqrt_u128(__uint128_t n)
{
    __uint128_t low  = 0;
    __uint128_t high = 1;
    while (high * high <= n)
        high <<= 1;
    low = high >> 1;
    while (high - low > 1) {
        __uint128_t mid = low + ((high - low) >> 1);
        if (mid * mid <= n)
            low = mid;
        else
            high = mid;
    }
    return (idx_t)low;
}

/* Lower Wythoff sequence A000201.
   Input m ≥ 1.  Returns ⌊(m+1)·φ⌋.  */
static seq_t lower_wythoff(idx_t m)
{
    /* x = m+1.  Compute ⌊x·√5⌋ by integer square root of 5·x². */
    __uint128_t x  = ( __uint128_t)m + 1;          /* x = m+1  */
    __uint128_t xx = x * x;                       /* x²      */
    __uint128_t fx = 5 * xx;                      /* 5·x²    */
    idx_t s = isqrt_u128(fx);                     /* ⌊√(5·x²)⌋ = ⌊x·√5⌋ */

    /* ⌊(x + ⌊x·√5⌋)/2⌋ gives ⌊x·φ⌋. */
    return ((seq_t)(x + s)) >> 1;
}

seq_t a(idx_t n)
{
    idx_t k = (n + 1) / 2;          /* ceil(n/2) */
    seq_t sum = 0;

    /* Lucas numbers: L1=1, L2=3 */
    seq_t L1 = 1, L2 = 3;

    for (idx_t i = 1; i <= k; ++i) {
        seq_t L;
        if (i == 1)
            L = L1;
        else if (i == 2)
            L = L2;
        else {
            seq_t tmp = L1 + L2;
            L1 = L2;
            L2 = tmp;
            L = tmp;
        }

        idx_t m = n - i + 1;          /* argument for A000201 */
        seq_t t_val = lower_wythoff(m);

        sum += L * t_val;
    }
    return sum;
}
