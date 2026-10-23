/*
 * A045213  Numbers whose base-5 representation contains exactly two 0's and
 *          three 4's.
 * https://oeis.org/A045213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow3_int128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A045213:
   nth smallest integer whose base‑5 representation contains exactly two
   0's and three 4's.  The algorithm builds the number digit by digit
   using combinatorial ranking. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow3_int128.h"

/* binomial coefficient C(n,k) in unsigned __int128 */
static __int128 binom_int128(int n, int k)
{
    if (k < 0 || k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    __int128 res = 1;
    for (int i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

/* number of valid length‑L numbers */
static __int128 count_len(int L)
{
    if (L < 5)
        return 0;
    __int128 A = binom_int128(L - 1, 2);          /* choose 2 zeros (not at 1st) */
    __int128 B = binom_int128(L - 3, 2);          /* choose 2 of remaining for fours when 1st is 4 */
    __int128 Cc = binom_int128(L - 2, 3);         /* choose 3 fours after zeros */
    __int128 term = ( (__int128)3 * Cc - (__int128)2 * B );
    return A * pow3_int128(L - 5) * term;
}

/* Construct the nth number of length L (1‑based rank). */
static seq_t build_len_nth(int L, idx_t rank)
{
    int zeros_rem = 2;
    int fours_rem  = 3;
    int positions_left = L;
    seq_t value = 0;

    for (int pos = 1; pos <= L; ++pos) {
        int start_digit = (pos == 1) ? 1 : 0;
        for (int d = start_digit; d <= 4; ++d) {
            if (d == 0 && zeros_rem == 0) continue;
            if (d == 4 && fours_rem  == 0) continue;

            int zeros_left = zeros_rem - (d == 0);
            int fours_left = fours_rem  - (d == 4);
            int positions_left_after = positions_left - 1;
            int others_left = positions_left_after - zeros_left - fours_left;
            if (others_left < 0) continue;   /* cannot satisfy remaining constraints */

            __int128 cnt = binom_int128(positions_left_after, zeros_left);
            cnt *= binom_int128(positions_left_after - zeros_left, fours_left);
            cnt *= pow3_int128(others_left);

            __int128 curr_rank = rank;
            if (cnt >= curr_rank) {
                value = value * 5 + (seq_t)d;
                zeros_rem = zeros_left;
                fours_rem = fours_left;
                positions_left = positions_left_after;
                break;
            } else {
                rank -= (idx_t)cnt;
            }
        }
    }
    return value;
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1, but avoid UB */

    __int128 cumulative = 0;
    int L = 4;

    /* find length L such that cumulative + count_len(L) >= n */
    while (1) {
        ++L;
        __int128 cnt = count_len(L);
        if (cumulative + cnt >= (__int128)n)
            break;
        cumulative += cnt;
    }

    idx_t rank_in_L = n - (idx_t)cumulative;   /* 1‑based rank within length L */
    return build_len_nth(L, rank_in_L);
}
