/*
 * A045061  Numbers whose base-4 representation contains exactly four 0's and
 *          four 2's.
 * https://oeis.org/A045061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   This function generates the nth number in base‑4 that contains *
 *            exactly four 0's and four 2's, sorted in increasing decimal
 *            value. * The sequence starts at a(1) = 32810
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>


/* generic nCk for k <= 4 using 128‑bit arithmetic */
static unsigned __int128 comb(uint64_t n, int k)
{
    if (k < 0 || (uint64_t)k > n) return 0;
    if (k > (int)n - k) k = (int)n - k;
    unsigned __int128 r = 1;
    for (int i = 1; i <= k; ++i) {
        r = r * (n - k + i);
        r /= i;
    }
    return r;
}

/* number of ways to fill m remaining positions with exactly
 * zeros zeros and twos twos (the rest are 1 or 3) */
static unsigned __int128 suffix_count(uint64_t m, int zeros, int twos)
{
    if (zeros < 0 || twos < 0) return 0;
    if ((int)m - zeros - twos < 0) return 0;
    if (m == 0) return (zeros == 0 && twos == 0) ? 1 : 0;
    unsigned __int128 c1 = comb(m, zeros);
    unsigned __int128 c2 = comb(m - zeros, twos);
    unsigned __int128 pow2 = (unsigned __int128)1 << (m - zeros - twos);
    return c1 * c2 * pow2;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* Find the length L of the n‑th number */
    unsigned __int128 cumulative = 0;
    int L;
    for (L = 8; ; ++L) {
        /* count of length L numbers:
         * case 1 – MS digit is 1 or 3
         * case 2 – MS digit is 2
         */
        unsigned __int128 cnt1 = comb(L - 1, 4) * comb(L - 5, 4);
        unsigned __int128 cnt2 = comb(L - 1, 3) * comb(L - 4, 4);
        unsigned __int128 pow = (unsigned __int128)1 << (L - 8);
        unsigned __int128 total = (cnt1 + cnt2) * pow;
        if (cumulative + total >= n) break;
        cumulative += total;
    }

    /* offset (1‑based) within this length */
    idx_t offset = n - (idx_t)cumulative;

    /* Build the base‑4 number digit by digit */
    seq_t value = 0;
    int zeros_left = 4;
    int twos_left  = 4;

    for (int pos = L - 1; pos >= 0; --pos) {
        for (int digit = 0; digit <= 3; ++digit) {
            if (pos == L - 1 && digit == 0) continue;        /* MS digit not zero */
            if (digit == 0 && zeros_left == 0) continue;
            if (digit == 2 && twos_left  == 0) continue;

            int z1 = zeros_left - (digit == 0 ? 1 : 0);
            int t1 = twos_left  - (digit == 2 ? 1 : 0);
            unsigned __int128 cnt = suffix_count(pos, z1, t1);

            if (offset > cnt) {
                offset -= (idx_t)cnt;            /* skip all numbers with this prefix */
            } else {
                /* choose this digit */
                value = value * (seq_t)4 + (seq_t)digit;
                zeros_left = z1;
                twos_left  = t1;
                break;
            }
        }
    }
    return value;
}
