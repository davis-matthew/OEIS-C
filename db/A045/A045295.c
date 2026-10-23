/*
 * A045295  Numbers whose base-5 representation contains no 3's and exactly
 *          three 4's.
 * https://oeis.org/A045295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A045295
 *  Numbers whose base-5 representation contains no 3's
 *  and exactly three 4's
 *
 *  The implementation generates the n‑th term by combinatorial
 *  unranking: we first determine the length L of the base‑5
 *  representation that contains the n‑th number, then we build
 *  the digits from most significant to least significant,
 *  always choosing the smallest digit that keeps the remaining
 *  rank within the possible completions.
 */

#include <stdint.h>

/* ------------- helper functions ----------------------------------- */

/* compute 3^e, clamped to UINT64_MAX */
static uint64_t pow3_uint(unsigned e)
{
    __int128 r = 1;
    for (unsigned i = 0; i < e; ++i) {
        r *= 3;
        if (r > (__int128)UINT64_MAX) return UINT64_MAX;
    }
    return (uint64_t)r;
}

/* binomial coefficient C(n,k), clamped to UINT64_MAX */
static uint64_t binom_uint(unsigned n, unsigned k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    __int128 r = 1;
    for (unsigned i = 1; i <= k; ++i) {
        r = r * (n - k + i) / i;
        if (r > (__int128)UINT64_MAX) return UINT64_MAX;
    }
    return (uint64_t)r;
}

/* number of completions for remaining (rem) positions
 * with exactly (fours) digits equal to 4
 */
static uint64_t suffix_count(uint64_t rem, uint64_t fours)
{
    if (fours > rem) return 0;
    __int128 c = (__int128)binom_uint((unsigned)rem, (unsigned)fours);
    c *= pow3_uint((unsigned)(rem - fours));
    if (c > (__int128)UINT64_MAX) return UINT64_MAX;
    return (uint64_t)c;
}

/* number of admissible numbers with a fixed length L */
static uint64_t count_len(uint64_t L)
{
    if (L < 3) return 0;
    uint64_t total = 0;

    /* case A: first digit is 4 */
    if (L >= 3) {
        uint64_t c = suffix_count(L - 1, 2); /* choose 2 more 4's */
        total += c;
    }

    /* case B: first digit is 1 or 2 */
    if (L >= 4) {
        uint64_t c = suffix_count(L - 1, 3); /* all 3 fours in remaining positions */
        total += 2 * c; /* first digit can be 1 or 2 */
    }
    return total;
}

/* ------------- main sequence function --------------------------- */

seq_t a(idx_t n)
{
    /* find length L such that the n-th number lies in that block */
    idx_t remaining = n;
    uint64_t L = 3;
    while (1) {
        uint64_t cnt = count_len(L);
        if (remaining <= cnt)
            break;
        remaining -= cnt;
        ++L;
    }

    /* unrank within the block of length L */
    idx_t rank = remaining;          /* 1‑based rank inside this block */
    uint64_t remaining_fours = 3;     /* fours still to place */
    seq_t value = 0;

    for (uint64_t pos = 1; pos <= L; ++pos) {
        unsigned cand[4];
        int candcnt;
        if (pos == 1) {
            candcnt = 3;
            cand[0] = 1; cand[1] = 2; cand[2] = 4;
        } else {
            candcnt = 4;
            cand[0] = 0; cand[1] = 1; cand[2] = 2; cand[3] = 4;
        }

        for (int i = 0; i < candcnt; ++i) {
            unsigned d = cand[i];
            uint64_t used_fours = remaining_fours;
            if (d == 4) --used_fours;
            if (used_fours > L - pos) continue; /* cannot fit */

            uint64_t completions = suffix_count(L - pos, used_fours);

            if (rank <= completions) {
                value = value * 5 + d;
                remaining_fours = used_fours;
                break;
            } else {
                rank -= completions;
            }
        }
    }

    return value;
}
