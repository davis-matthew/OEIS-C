/*
 * A045186  Numbers whose base-5 representation contains exactly three 0's and
 *          two 2's.
 * https://oeis.org/A045186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer whose base‑5 representation contains
 *            exactly three 0’s and two 2’s, with no leading zero.
 *            Computation: determine length L by cumulative counts, then
 *            construct the L‑digit base‑5 number digit by digit using
 *            combinatorial counting
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include minimal headers */
#include <stdint.h>
#include <stddef.h>

/* arbitrary‑precision helpers */
static inline unsigned __int128
binom_u128(unsigned n, unsigned k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    unsigned __int128 res = 1;
    for (unsigned i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

static inline unsigned __int128
pow3_u128(unsigned exp)
{
    unsigned __int128 res = 1;
    for (unsigned i = 0; i < exp; ++i) res *= 3;
    return res;
}

/* count of L‑digit numbers (first digit ≠0) with exactly
   3 zeros and 2 twos */
static inline unsigned __int128
count_len(unsigned L)
{
    if (L < 5) return 0;
    unsigned __int128 c1 = binom_u128(L - 3, 2);   /* C(L-3,2) */
    unsigned __int128 c2 = binom_u128(L - 1, 3);   /* C(L-1,3) */
    unsigned __int128 c3 = pow3_u128(L - 5);       /* 3^(L-5) */
    return c1 * c2 * c3;
}

/* number of completions for given remaining positions
   rem_len, zeros z, twos t (first digit already fixed) */
static inline unsigned __int128
count_completions(unsigned rem_len,
                  unsigned z, unsigned t,
                  unsigned __int128 cap) /* cap = k+1 for early exit */
{
    if (z > rem_len || t > rem_len - z) return 0;
    unsigned others = rem_len - z - t;
    unsigned __int128 cnt = 1;
    cnt *= binom_u128(rem_len, z);
    if (cnt > cap) return cnt;
    cnt *= binom_u128(rem_len - z, t);
    if (cnt > cap) return cnt;
    cnt *= pow3_u128(others);
    return cnt;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but return 0 for safety */

    /* Find the length L for the nth term */
    unsigned L = 5;
    unsigned __int128 cum = 0;
    unsigned __int128 target = (unsigned __int128)n - 1; /* zero‑based offset */
    while (1) {
        unsigned __int128 cntL = count_len(L);
        if (cum + cntL > target) break;
        cum += cntL;
        ++L;
    }
    unsigned __int128 offset = target - cum; /* within this length, zero‑based */

    /* Build the number digit by digit */
    unsigned rem_z = 3, rem_t = 2;
    unsigned __int128 rem_offset = offset;
    unsigned __int128 rem_len = L;
    seq_t value = 0;

    for (unsigned pos = 0; pos < L; ++pos) {
        unsigned rem_len_current = L - pos;
        /* iterate candidate digits in ascending order */
        for (int d = 0; d <= 4; ++d) {
            if (pos == 0 && d == 0) continue;            /* no leading zero */
            if (d == 0 && rem_z == 0) continue;
            if (d == 2 && rem_t == 0) continue;
            unsigned nz = rem_z - (d == 0);
            unsigned nt = rem_t - (d == 2);
            unsigned rem_len_next = rem_len_current - 1;
            if (nz > rem_len_next || nt > rem_len_next - nz) continue;

            unsigned __int128 cnt = count_completions(rem_len_next, nz, nt,
                                                       rem_offset + 1);
            if (cnt <= rem_offset) {
                rem_offset -= cnt;
                continue;   /* skip this branch */
            }

            /* choose this digit */
            value = value * 5 + (seq_t)d;
            rem_z = nz;
            rem_t = nt;
            break;
        }
    }

    return value;
}
