/*
 * A171180  a(n) = (4*n + 1)^(1/2)/(4*n + 1)*((1 - p)*q^n - (1 - q)*p^n),
 *          where p = (1 - (4*n + 1)^(1/2))/2 and q = (1 + (4*n + 1)^(1/2))/2.
 * https://oeis.org/A171180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n-k, k) * n^k
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   idxs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/idxs.h"
/* This implements the combinatorial sum representation of OEIS A171180. */

#ifndef __cplusplus
/* C implementation: use 128‑bit multiplication to avoid overflow on built‑in types. */
static inline seq_t safe_mul(seq_t a, seq_t b) {
    return (seq_t)((unsigned __int128)a * (unsigned __int128)b);
}
#else
/* C++17 implementation: use a type‑dependent 128‑bit multiplication when possible. */
template <typename T>
static inline T safe_mul(T a, T b) {
    if constexpr (sizeof(T) <= sizeof(unsigned long long))
        return (T)((unsigned __int128)a * (unsigned __int128)b);
    else
        return a * b;
}
#endif

seq_t a(idx_t n)
{
    idx_t maxk = n / 2;
    seq_t binom = 1;    /* binomial(n-k, k) */
    seq_t power = 1;    /* n^k */
    seq_t sum   = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* Add current term */
        sum += safe_mul(binom, power);

        if (k == maxk) break;     /* last term reached */

        /* Update power: n^(k+1) */
        power = safe_mul(power, idxs(n));

        /* Update binomial:
           binom_{k+1} = binom_k * (n-2k)*(n-2k-1) / ((n-k)*(k+1))
           We split into two successive multiplies/divides to keep numbers smaller. */
        uint64_t prod1 = n - 2 * k;
        uint64_t prod2 = n - 2 * k - 1;
        uint64_t den1  = n - k;
        uint64_t den2  = k + 1;

        binom = safe_mul(binom, idxs(prod1));
        binom = binom / idxs(den1);

        binom = safe_mul(binom, idxs(prod2));
        binom = binom / idxs(den2);
    }

    return sum;
}
