/*
 * A043432  Numbers having four 2's in base 8.
 * https://oeis.org/A043432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth base‑8 number that contains exactly four digits ‘2’
 * Range:     u8 none, u16 n <= 70, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 7^e, e ≥ 0 */
static unsigned __int128 pow_uint(unsigned base, idx_t exp)
{
    unsigned __int128 r = 1;
    for (idx_t i = 0; i < exp; ++i) r *= base;
    return r;
}

/* Binomial coefficient C(n,k) in 128‑bit arithmetic */
static unsigned __int128 comb(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

/* Number of length‑L base‑8 numbers with exactly four 2’s,
 * first digit non‑zero. */
static unsigned __int128 count_len(idx_t L)
{
    if (L < 4) return 0;
    unsigned __int128 ways = 0;

    /* first digit is 2: choose 3 more twos in the remaining L‑1 positions
       and fill the other positions with 7 possibilities (0,1,3,…,7). */
    if (L - 1 >= 3)
        ways += comb(L - 1, 3) * pow_uint(7, L - 4);

    /* first digit is non‑zero, not 2 (6 choices).  Choose 4 twos in rest,
       other positions again have 7 possibilities. */
    if (L - 1 >= 4)
        ways += 6 * comb(L - 1, 4) * pow_uint(7, L - 5);

    return ways;
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    unsigned __int128 cumulative = 0;
    idx_t L = 4;                 /* minimal length */

    /* Find the length of the desired number. */
    while (1) {
        unsigned __int128 cnt = count_len(L);
        if (cumulative + cnt >= n) break;
        cumulative += cnt;
        ++L;
    }

    idx_t rank = n - (idx_t)cumulative - 1;   /* 0‑based within length L */

    seq_t num = 0;
    unsigned twos_left = 4;

    /* Construct the number digit by digit. */
    for (idx_t pos = 0; pos < L; ++pos) {
        unsigned positions_left = L - pos - 1;
        unsigned first_min = (pos == 0) ? 1 : 0;

        for (int d = (int)first_min; d <= 7; ++d) {
            unsigned t2 = twos_left;
            if (d == 2) {
                if (t2 == 0) continue;
                --t2;
            }
            if (t2 > positions_left) continue;

            unsigned __int128 ways =
                comb(positions_left, t2) * pow_uint(7, positions_left - t2);

            if ((unsigned __int128)rank >= ways) {
                rank -= (unsigned __int128)ways;
                continue;
            } else {
                num = num * ((seq_t)8) + (seq_t)d;
                twos_left = t2;
                break;
            }
        }
    }

    return num;
}
