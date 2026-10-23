/*
 * A045306  Numbers whose base-5 representation contains exactly three 3's and
 *          two 4's.
 * https://oeis.org/A045306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045306: base‑5 numbers containing exactly three 3's and two 4's.
   Computed by combinatorial ranking of the base‑5 representation.  */

#include <stdint.h>

/* Binomial coefficient C(n,k) for 0 ≤ k ≤ n, computed exactly in 64‑bit. */
static unsigned long long binom(unsigned int n, unsigned int k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    unsigned long long r = 1;
    for (unsigned int i = 1; i <= k; ++i)
        r = r * (n - k + i) / i;
    return r;
}

/* Power of 3: 3^e, 0 ≤ e ≤ 20 (fits in 64‑bit for all relevant e). */
static unsigned long long pow3(unsigned int e)
{
    unsigned long long r = 1;
    for (unsigned int i = 0; i < e; ++i) r *= 3ULL;
    return r;
}

/* Number of length‑L base‑5 strings (leading zero forbidden) with exactly
   three 3’s and two 4’s.  */
static unsigned long long count_length(unsigned int L)
{
    if (L < 5) return 0;
    unsigned long long total = binom(L, 3) * binom(L - 3, 2) * pow3(L - 5);
    unsigned long long lead = 0;
    if (L >= 6)
        lead = binom(L - 1, 3) * binom(L - 4, 2) * pow3(L - 6);
    return total - lead;
}

/* Number of completions for the remaining positions when we have
   positions remaining, need3 three‑digits and need4 four‑digits left.
   Here leading zeros are allowed, as we are inside the number. */
static unsigned long long count_completions(unsigned int positions,
                                            unsigned int need3,
                                            unsigned int need4)
{
    if (need3 + need4 > positions) return 0;
    return binom(positions, need3) * binom(positions - need3, need4) *
           pow3(positions - need3 - need4);
}

/* Main function implementing a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /*  offset starts at 1; undefined for n==0 */
    unsigned long long rem = (unsigned long long)n;
    unsigned int L = 5;               /* minimal length (3+2 digits) */

    /* Find the length L that contains the n‑th term. */
    while (1) {
        unsigned long long cnt = count_length(L);
        if (rem <= cnt) break;
        rem -= cnt;
        ++L;
    }

    /* Index (0‑based) within the block of length L. */
    unsigned long long idx = rem - 1;
    unsigned int need3 = 3, need4 = 2;
    seq_t value = 0;

    /* Build the number digit by digit using combinatorial ranking. */
    for (unsigned int pos = 0; pos < L; ++pos) {
        for (unsigned int d = 0; d <= 4; ++d) {
            if (pos == 0 && d == 0) continue;          /* avoid leading zero */
            if (d == 3 && need3 == 0) continue;        /* no spare 3’s */
            if (d == 4 && need4 == 0) continue;        /* no spare 4’s */

            unsigned int n3 = need3, n4 = need4;
            if (d == 3) n3--;
            if (d == 4) n4--;

            unsigned int rem_pos = L - pos - 1;
            if (n3 + n4 > rem_pos) continue;          /* impossible to finish */

            unsigned long long completions =
                count_completions(rem_pos, n3, n4);

            if (idx < completions) {
                value = value * 5 + (seq_t)d;
                need3 = n3;
                need4 = n4;
                break;          /* proceed to next position */
            } else {
                idx -= completions;
            }
        }
    }
    return value;
}
