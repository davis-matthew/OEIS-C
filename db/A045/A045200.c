/*
 * A045200  Numbers whose base-5 representation contains exactly three 0's and
 *          one 3.
 * https://oeis.org/A045200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045200:  Numbers whose base‑5 representation contains exactly
 * three 0's and one 3.  The implementation builds the nth term by
 * combinatorial ranking: it first determines the required length L of
 * the base‑5 representation, then constructs the digits from most
 * significant to least significant by repeatedly counting how many
 * completions are possible with each candidate digit.  The algorithm
 * runs in O(L * 5) time with L ≤ 8 for n ≤ 10 000, well within the
 * required limits. */

#include <stdint.h>

/* Binomial coefficient – works for small n (≤ 64 in practice). */
static inline idx_t comb(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    if (k == 0) return 1;
    __int128 prod = 1;
    for (idx_t i = 1; i <= k; ++i)
        prod = prod * (n - k + i) / i;
    return (idx_t)prod;
}

/* 3^e for small e. */
static inline idx_t pow3(idx_t e)
{
    idx_t r = 1;
    while (e--) r *= 3;
    return r;
}

/* Number of qualifying numbers of exact length L (L ≥ 4). */
static inline idx_t count_len(idx_t L)
{
    /* C(L-1,3) * (L-3) * 3^(L-4) */
    return comb(L - 1, 3) * (L - 3) * pow3(L - 4);
}

/* Count completions for given remaining positions, zeros and threes. */
static inline idx_t count_completions(idx_t rem,
                                      idx_t zeros, idx_t threes)
{
    if (zeros > rem || threes > rem - zeros) return 0;
    return comb(rem, zeros) * comb(rem - zeros, threes) *
           pow3(rem - zeros - threes);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined for n = 0 */

    /* Find the required length L. */
    idx_t L = 4;
    idx_t cumulative = 0;
    while (1) {
        idx_t cnt = count_len(L);
        if (cumulative + cnt >= n) break;
        cumulative += cnt;
        ++L;
    }

    idx_t r = n - cumulative - 1;          /* 0‑based rank inside length L */
    int zeros_left = 3;
    int threes_left = 1;
    seq_t value = 0;

    for (int pos = 0; pos < (int)L; ++pos) {
        int d_start = (pos == 0 ? 1 : 0);
        for (int d = d_start; d <= 4; ++d) {
            /* Skip impossible choices. */
            if (d == 0 && zeros_left == 0) continue;
            if (d == 3 && threes_left == 0) continue;

            int zeros_tmp = zeros_left;
            int threes_tmp = threes_left;
            if (d == 0) --zeros_tmp;
            else if (d == 3) --threes_tmp;

            idx_t rem = L - pos - 1;

            if ((idx_t)zeros_tmp > rem) continue;
            if ((idx_t)threes_tmp > rem - zeros_tmp) continue;

            idx_t ways = count_completions(rem, zeros_tmp, threes_tmp);
            if (r < ways) {                  /* choose this digit */
                value = value * 5 + (seq_t)d;
                zeros_left = zeros_tmp;
                threes_left = threes_tmp;
                break;
            } else {
                r -= ways;
            }
        }
    }
    return value;
}
