/*
 * A025114  s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2 ], s =
 *          (odd natural numbers), t = A001950 (upper Wythoff sequence).
 * https://oeis.org/A025114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} (2·i−1)·⌊(n−i+1+1)·φ²⌋ * where φ² =
 *            (3+√5)/2 and the floor is over integers. * The inner term
 *            floor((k+1)·φ²) is computed as * floor( (3·(k+1) +
 *            floor(√(5·(k+1)²)) ) / 2 ). * * The function uses only integer
 *            arithmetic suitable for any * seq_t type supported by the
 *            buil...
 * Range:     u8 n <= 7, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root for unsigned __int128.
 * Returns floor( sqrt(x) ). */
static unsigned long long uint128_sqrt(unsigned __int128 x)
{
    unsigned long long r = 0;
    unsigned long long bit = 1ULL << 63;   /* highest bit for 64‑bit */
    while (bit) {
        unsigned long long next = r | bit;
        unsigned __int128 sq = (unsigned __int128)next * next;
        if (sq <= x)
            r = next;
        bit >>= 1;
    }
    return r;
}

/* Compute ⌊(k+1)·φ²⌋ for integer k ≥ 0. */
static seq_t t_val(idx_t k)
{
    unsigned long long kp1 = (unsigned long long)(k + 1);
    unsigned __int128 sq = (unsigned __int128)5 * kp1 * kp1;
    unsigned long long sqrt_part = uint128_sqrt(sq);

    unsigned __int128 numerator = (unsigned __int128)3 * kp1 + sqrt_part;
    return (seq_t)(numerator / 2);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */
    idx_t k = (n + 1) / 2;                     /* floor((n+1)/2) */
    seq_t sum = 0;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t s = (seq_t)(2 * i - 1);          /* odd natural numbers */
        idx_t idx = n - i + 1;                /* argument for t */
        seq_t t = t_val(idx);                 /* floor((idx+1)*φ²) */
        sum += s * t;
    }
    return sum;
}
