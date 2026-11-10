/*
 * A085194  Terms of A085193 halved. The repeating part in the first
 *          differences of A057520.
 * https://oeis.org/A085194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A085193(n)/2, the n-th gap (halved) between consecutive
 *            Dyck-word numbers in the stable prefix of the length-2m blocks
 *            of A057520. Computed by unranking Dyck words n and n+1 in a
 *            block with Catalan(m) > n+1
 * Range:     u8 n <= 12, u16 n <= 1428, u32 n <= 1429, u64 n <= 1429,
 *            u128 n <= 1429, big any n
 * Validated: standard, pass, 118 terms through n = 1382 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned __int128 A085194_binom(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;
    unsigned __int128 r = 1;
    for (idx_t i = 1; i <= K; ++i)
        r = r * (unsigned __int128)(N - K + i) / i;
    return r;
}

/* paths of rem steps from balance bal to 0, never below 0 */
static unsigned __int128 A085194_count(idx_t rem, idx_t bal)
{
    if (bal > rem) return 0;
    if ((rem - bal) & 1) return 0;
    idx_t t = (rem + bal) / 2;
    return A085194_binom(rem, t) - A085194_binom(rem, t + 1);
}

static seq_t A085194_unrank(idx_t m, unsigned __int128 k)
{
    idx_t rem = 2 * m - 1;
    idx_t bal = 1;
    seq_t val = (seq_t)1;
    while (rem > 0)
    {
        if (bal == 0)
        {
            val = (val << 1) | 1;
            ++bal;
        }
        else
        {
            unsigned __int128 c0 = A085194_count(rem - 1, bal - 1);
            if (k < c0)
            {
                val = val << 1;
                --bal;
            }
            else
            {
                val = (val << 1) | 1;
                k -= c0;
                ++bal;
            }
        }
        --rem;
    }
    return val;
}

seq_t a(idx_t n)
{
    unsigned __int128 target = (unsigned __int128)n + 1;
    idx_t m = 1;
    unsigned __int128 C = 1; /* Catalan(1) = 1 */
    while (C <= target)
    {
        C = C * (unsigned __int128)(2 * (2 * m + 1)) / (unsigned __int128)(m + 2);
        ++m;
    }
    seq_t w0 = A085194_unrank(m, (unsigned __int128)n);
    seq_t w1 = A085194_unrank(m, (unsigned __int128)n + 1);
    return (w1 - w0) >> 1;
}
