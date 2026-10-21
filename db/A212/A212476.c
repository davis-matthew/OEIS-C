/*
 * A212476  Number of 0..3 arrays of length 2*n+4 with sum no more than 3*n in
 *          any length 2n subsequence (=50% duty cycle).
 * https://oeis.org/A212476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by splitting the 2n+4‑array into its first * four
 *            entries, a middle block of length 2n‑4, and its last four *
 *            entries. For each possible offset of the middle block a *
 *            combinatorial count of sequences with sum ≤ 3n+offset is added *
 *            weighted by the number of (first, last) pairs that...
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binom.h"

static uint64_t pattern_counts[25];
static int pattern_counts_ready = 0;

/* Precompute pattern_counts once. */
static void init_pattern_counts(void)
{
    for (int a0 = 0; a0 < 4; ++a0)
        for (int a1 = 0; a1 < 4; ++a1)
            for (int a2 = 0; a2 < 4; ++a2)
                for (int a3 = 0; a3 < 4; ++a3)
                {
                    int SA  = a0 + a1 + a2 + a3;
                    int SA1 = a0;
                    int SA2 = a0 + a1;
                    int SA3 = a0 + a1 + a2;
                    for (int b0 = 0; b0 < 4; ++b0)
                        for (int b1 = 0; b1 < 4; ++b1)
                            for (int b2 = 0; b2 < 4; ++b2)
                                for (int b3 = 0; b3 < 4; ++b3)
                                {
                                    int SB  = b0 + b1 + b2 + b3;
                                    int B1  = b0;
                                    int B2  = b0 + b1;
                                    int B3  = b0 + b1 + b2;
                                    int delta0 = 0;
                                    int delta1 = SA1 - B1;
                                    int delta2 = SA2 - B2;
                                    int delta3 = SA3 - B3;
                                    int delta4 = SA  - SB;
                                    int min_delta = delta0;
                                    if (delta1 < min_delta) min_delta = delta1;
                                    if (delta2 < min_delta) min_delta = delta2;
                                    if (delta3 < min_delta) min_delta = delta3;
                                    if (delta4 < min_delta) min_delta = delta4;
                                    int O = min_delta - SA;          /* O in [-24,0] */
                                    pattern_counts[O + 24] += 1ULL;
                                }
                }
    pattern_counts_ready = 1;
}

/* Count of sequences of length N (alphabet 0..3) with total sum <= U. */
static seq_t count_leq(idx_t N, idx_t U)
{
    if ((int64_t)U < 0) return 0;
    if (N == 0) return 1;              /* empty sequence */
    seq_t res = 0;
    idx_t max_t = U / 4;                 /* floor(U/4) */
    for (idx_t t = 0; t <= max_t; ++t)
    {
        seq_t c1 = binom(N, t);
        if (c1 == 0) continue;
        idx_t a = U - 4 * t + N;         /* binomial arguments must be >= N */
        seq_t c2 = binom(a, N);
        seq_t term = c1 * c2;
        if (t & 1) res -= term;
        else       res += term;
    }
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!pattern_counts_ready) init_pattern_counts();

    /* ----- n = 1: brute force enumeration (4^6 = 4096) ----- */
    if (n == 1)
    {
        seq_t cnt = 0;
        const idx_t L   = 6;      /* 2*n + 4 */
        const idx_t win = 2;      /* window length = 2*n */
        const uint32_t max_mask = 1u << (2 * L);  /* 4^6 = 2^(2L) */

        for (uint32_t mask = 0; mask < max_mask; ++mask)
        {
            uint8_t a[6];
            uint32_t tmp = mask;
            for (idx_t k = 0; k < L; ++k)
            {
                a[k] = tmp & 3u;   /* extract 2 bits -> value 0..3 */
                tmp >>= 2;
            }
            bool ok = true;
            for (idx_t w = 0; w + win <= L; ++w)
            {
                idx_t sum = 0;
                for (idx_t j = w; j < w + win; ++j) sum += a[j];
                if (sum > 3 * n) { ok = false; break; }
            }
            if (ok) ++cnt;
        }
        return cnt;
    }

    /* ----- n >= 2: analytic counting via pattern offsets ----- */
    idx_t N_middle = 2 * n - 4;   /* length of middle block */
    seq_t total = 0;

    for (int o = -24; o <= 0; ++o)
    {
        idx_t patterns = pattern_counts[o + 24];
        if (patterns == 0) continue;

        int64_t U_signed = (int64_t)3 * (int64_t)n + o;
        if (U_signed < 0) continue;           /* no valid sequences for this offset */
        idx_t U = (idx_t)U_signed;

        seq_t g = count_leq(N_middle, U);
        if (g == 0) continue;

        total += g * (seq_t)patterns;
    }
    return total;
}
