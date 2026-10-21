/*
 * A056350  Number of primitive (period n) bracelets using exactly four
 *          different colored beads.
 * https://oeis.org/A056350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * B_4(n/d) (A056350) * where B_4(m) =
 *            number of primitive bracelets of length m * with exactly four
 *            colors (A056344)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow_uint_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint_u64.h"

/* ------------------------------------------------------------
 * Möbius function μ(n) for 1 ≤ n ≤ 10000 (works for any n)
 * ------------------------------------------------------------ */
static int mu_of(idx_t n)
{
    if (n == 1) return 1;
    int odd_cnt = 0;
    idx_t x = n;
    if (x % 2 == 0)
    {
        odd_cnt++;
        x /= 2;
        if (x % 2 == 0) return 0;     /* square factor 2² */
    }
    for (idx_t p = 3; p * p <= x; p += 2)
    {
        if (x % p == 0)
        {
            odd_cnt++;
            x /= p;
            if (x % p == 0) return 0; /* squared prime factor */
        }
    }
    if (x > 1) odd_cnt++;            /* remaining prime factor */
    return (odd_cnt % 2 == 0) ? 1 : -1;
}

/* ------------------------------------------------------------
 * Euler's totient φ(n)
 * ------------------------------------------------------------ */
static seq_t phi_of(idx_t n)
{
    seq_t result = (seq_t)n;
    idx_t x = n;
    if (x % 2 == 0)
    {
        result = result * (seq_t)(2 - 1) / 2;
        while (x % 2 == 0) x /= 2;
    }
    for (idx_t p = 3; p * p <= x; p += 2)
    {
        if (x % p == 0)
        {
            result = result * (seq_t)(p - 1) / p;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1)
        result = result * (seq_t)(x - 1) / (seq_t)x;
    return result;
}

/* ------------------------------------------------------------
 * Number of bracelets with at most k colors (non‑primitive)
 * ------------------------------------------------------------ */
static seq_t bracelets_at_most(idx_t n, uint64_t k)
{
    /* sum_{d|n} φ(d) * k^{n/d}                                 */
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t d1 = d;
            idx_t d2 = n / d;
            seq_t phi1 = phi_of(d1);
            sum += phi1 * pow_uint_u64(k, n / d1);
            if (d1 != d2)
            {
                seq_t phi2 = phi_of(d2);
                sum += phi2 * pow_uint_u64(k, n / d2);
            }
        }
    }
    /* reflection terms                                       */
    if (n & 1U)         /* n odd */
    {
        sum += (seq_t)n * pow_uint_u64(k, (n + 1) / 2);
    }
    else                /* n even */
    {
        seq_t term = (seq_t)(n / 2) *
                     (pow_uint_u64(k, n / 2 + 1) + pow_uint_u64(k, n / 2));
        sum += term;
    }
    /* divide by 2n */
    seq_t denom = (seq_t)(2 * n);
    return sum / denom;
}

/* ------------------------------------------------------------
 * Number of bracelets with exactly four colors
 * ------------------------------------------------------------ */
static seq_t bracelets_exactly_four(idx_t n)
{
    const uint64_t comb[5] = {1ULL, 4ULL, 6ULL, 4ULL, 1ULL};
    seq_t result = 0;
    for (int i = 0; i <= 4; ++i)
    {
        uint64_t remaining_colors = 4 - (uint64_t)i;
        seq_t B = bracelets_at_most(n, remaining_colors);
        seq_t term = (seq_t)comb[i] * B;
        if (i & 1)
            result -= term;
        else
            result += term;
    }
    return result;
}

/* ------------------------------------------------------------
 * Main sequence function: A056350   (primitive bracelets)
 * ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t d1 = d;
            idx_t d2 = n / d;
            int mu1 = mu_of(d1);
            seq_t val1 = bracelets_exactly_four(n / d1);
            if (mu1 > 0) total += val1;
            else if (mu1 < 0) total -= val1;

            if (d1 != d2)
            {
                int mu2 = mu_of(d2);
                seq_t val2 = bracelets_exactly_four(n / d2);
                if (mu2 > 0) total += val2;
                else if (mu2 < 0) total -= val2;
            }
        }
    }
    return total;
}
