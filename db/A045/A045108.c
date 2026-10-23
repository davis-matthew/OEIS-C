/*
 * A045108  Numbers whose base-4 representation contains exactly four 1's and
 *          three 2's.
 * https://oeis.org/A045108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number whose base‑4 representation contains exactly four
 *            1's and three 2's. The algorithm generates numbers of increasing
 *            length and, for a given length, counts how many possibilities
 *            start with each possible digit. It uses combinatorial ranking to
 *            find the nth term without storing all terms
 * Range:     u8 none, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

//--- include helpers ----------------------------------------------------------
#include <stdint.h>   /* idx_t and seq_t are defined by the build */

//--- helper functions ----------------------------------------------------------
static idx_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

static idx_t pow2(idx_t e)
{
    return ((idx_t)1) << e;   /* e is small (≤ 20) */
}

static idx_t count_length(idx_t L)
{
    if (L < 7) return 0;
    return binom(L, 4) * binom(L - 4, 3) * pow2(L - 7);
}

static idx_t count_combinations(idx_t remaining, idx_t rem1, idx_t rem2)
{
    /* remaining positions to fill, rem1 ones left, rem2 twos left.
       The other positions (remaining - rem1 - rem2) can be 0 or 3.
    */
    return binom(remaining, rem1) *
           binom(remaining - rem1, rem2) *
           pow2(remaining - rem1 - rem2);
}

//--- main sequence function ----------------------------------------------------
seq_t a(idx_t n)
{
    /* Find the length L that contains the nth term. */
    idx_t cumulative = 0;
    idx_t L = 7;   /* minimal length to place 4 ones and 3 twos */
    while (1) {
        idx_t cnt = count_length(L);
        if (cumulative + cnt >= n) break;
        cumulative += cnt;
        ++L;
    }

    /* Rank of the term within numbers of length L (0‑based). */
    idx_t rank = n - cumulative - 1;

    idx_t rem1 = 4, rem2 = 3;
    idx_t remaining = L;
    idx_t val = 0;

    for (idx_t pos = 0; pos < L; ++pos) {
        for (int d = 0; d <= 3; ++d) {
            if (pos == 0 && d == 0) continue;          /* no leading 0 */
            if (d == 1 && rem1 == 0) continue;
            if (d == 2 && rem2 == 0) continue;

            idx_t nrem1 = rem1;
            idx_t nrem2 = rem2;
            if (d == 1) nrem1--;
            if (d == 2) nrem2--;

            idx_t new_rem = remaining - 1;
            idx_t combinations = count_combinations(new_rem, nrem1, nrem2);

            if (rank >= combinations) {
                rank -= combinations;   /* skip these many candidates */
            } else {
                val = val * 4 + d;
                rem1 = nrem1;
                rem2 = nrem2;
                remaining = new_rem;
                break;   /* move to next position */
            }
        }
    }

    return (seq_t)val;
}
