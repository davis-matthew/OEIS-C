/*
 * A045505  Convolution of A000108 (Catalan numbers) with A040075.
 * https://oeis.org/A045505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+5,4)*(4^(n+1) - binomial(2*(n+5),n+5)/70)/2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 24, u128 n <= 53,
 *            big any n
 * Helpers:   binomial, pow4_u128, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"
#include "../utils/pow4_u128.h"

/* Helper for 128‑bit binomial when seq_t is small */
static inline unsigned __int128 binom_u128(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;
    unsigned __int128 r = 1;
    for (idx_t i = 1; i <= k; ++i)
        r = r * (n - k + i) / i;
    return r;
}

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for small seq_t types (≤8 bytes) */
    if (sizeof(seq_t) <= sizeof(unsigned long long))
    {
        unsigned __int128 bin1   = binom_u128(n + 5, 4);
        unsigned __int128 pow4   = pow4_u128(n + 1);
        unsigned __int128 central = binom_u128(2 * (n + 5), n + 5);

        unsigned __int128 term     = 70 * pow4 - central;
        unsigned __int128 product  = bin1 * term;
        unsigned __int128 result   = product / 140;

        return (seq_t)result;   /* truncates if value exceeds seq_t */
    }

    /* For arbitrary‑precision seq_t use the provided utilities */
    seq_t bin1   = binomial(n + 5, 4);
    seq_t pow4   = seq_pow((seq_t)4, n + 1);
    seq_t central = binomial(2 * (n + 5), n + 5);

    seq_t term     = 70 * pow4 - central;
    seq_t product  = bin1 * term;
    seq_t result   = product / 140;

    return result;
}
