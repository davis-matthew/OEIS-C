/*
 * A018820  Numbers k that are the sum of m nonzero squares for all 1 <= m <=
 *          k - 14.
 * https://oeis.org/A018820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 120, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A018820:
   a(n) is the n-th square k^2 such that k^2 can be represented as
   a sum of m distinct **nonzero** squares for every 1 ≤ m ≤ k^2−14.
   Equivalent to: k has a prime divisor ≡1 (mod 4) and
   k^2 is the sum of three **positive** squares. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* maximum k we will examine – good enough for n up to ~1e4  */
#define MAX_K 35000u

/* helper arrays – allocated once at first call */
static uint64_t *two_sum_bits = NULL;  /* bitset of all sums a^2+b^2, 1≤a≤b≤MAX_K */
static uint32_t  spf[MAX_K + 1];        /* smallest prime factor for 0..MAX_K */
static bool      initialized = false;

/* local helper: set bit in bitset */
static inline void set_bit(uint64_t *bits, uint64_t pos)
{
    bits[pos >> 6] |= (1ULL << (pos & 63));
}
/* local helper: test bit in bitset */
static inline bool test_bit(const uint64_t *bits, uint64_t pos)
{
    return (bits[pos >> 6] >> (pos & 63)) & 1ULL;
}

/* local helper: prime sieve to fill spf */
static void build_spf(void)
{
    for (uint32_t i = 2; i <= MAX_K; ++i)
    {
        if (!spf[i])
        {
            spf[i] = i;
            if ((uint64_t)i * i <= MAX_K)
            {
                for (uint64_t j = (uint64_t)i * i; j <= MAX_K; j += i)
                    if (!spf[j]) spf[j] = i;
            }
        }
    }
}

/* local helper: does k have a prime factor ≡1 (mod 4)? */
static bool has_prime_1mod4(uint32_t k)
{
    if (k <= 1) return false;
    while (k > 1)
    {
        uint32_t p = spf[k];
        if (p % 4 == 1) return true;
        while (k % p == 0) k /= p;
    }
    return false;
}

/* local helper: build all sums of two positive squares up to MAX_K^2 */
static void build_two_sum_bits(void)
{
    uint64_t max_sq = (uint64_t)MAX_K * MAX_K;                          /* <= 1.225e9 */
    size_t   bitlen = (max_sq >> 6) + 1;                                 /* number of 64‑bit words */
    two_sum_bits = (uint64_t *)calloc(bitlen, sizeof(uint64_t));
    if (!two_sum_bits) {
        /* allocation failed – abort the program */
        abort();
    }

    for (uint32_t a = 1; a <= MAX_K; ++a)
    {
        uint64_t a2 = (uint64_t)a * a;
        for (uint32_t b = a; b <= MAX_K; ++b)
        {
            uint64_t sum = a2 + (uint64_t)b * b;
            if (sum > max_sq) break;
            set_bit(two_sum_bits, sum);
        }
    }
}

/* initialise once */
static void init(void)
{
    if (initialized) return;
    build_spf();
    build_two_sum_bits();
    initialized = true;
}

/* main exported function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 1, harmless */

    init();

    idx_t found = 0;
    for (uint32_t k = 1; k <= MAX_K; ++k)
    {
        /* condition 1 – prime divisor ≡ 1 (mod 4) */
        if (!has_prime_1mod4(k)) continue;

        /* condition 2 – k² is a sum of three positive squares   */
        uint64_t k2 = (uint64_t)k * k;
        bool ok = false;
        for (uint32_t t = 1; t < k; ++t)
        {
            uint64_t r = k2 - (uint64_t)t * t;   /* remainder for a²+b² */
            if (r == 0) continue;                /* would require a or b == 0 */
            if (test_bit(two_sum_bits, r))
            {
                ok = true;
                break;
            }
        }
        if (!ok) continue;

        /* found another term of the sequence */
        ++found;
        if (found == n)
            return (seq_t)k2;
    }

    /* if we run out of MAX_K bounds, fall back to 0 (should not happen for intended constraints) */
    return (seq_t)0;
}
