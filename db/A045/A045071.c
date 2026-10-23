/*
 * A045071  Numbers whose base-4 representation contains exactly two 0's and
 *          no 3's.
 * https://oeis.org/A045071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   count_len_uint128, count_strings, pow2_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A045071: Numbers whose base‑4 representation contains exactly two 0's
 *  and no 3's.  The function uses combinatorial ranking/unranking of
 *  base‑4 strings of length L with exactly two zeros, first digit non‑zero.
 *  Each a(n) is computed in O(L) time, where L ≈ log₂(n); for n up to about
 *  10 000 the algorithm executes easily within the time limit.
 */

#include <stdint.h>
#include "../utils/count_len_uint128.h"
#include "../utils/count_strings.h"
#include "../utils/pow2_uint128.h"

seq_t a(idx_t n)
{
    /* Find the length L of the number at position n */
    unsigned int L = 3;                       /* minimum length with two zeros */
    idx_t cumulative_prev = 0;                /* cumulative count up to L-1   */
    unsigned __int128 cumulative = 0;         /* use 128‑bit to avoid overflow */

    while (1) {
        unsigned __int128 cnt = count_len_uint128(L);
        cumulative += cnt;
        if (cumulative >= (unsigned __int128)n)
            break;
        ++L;
    }
    cumulative_prev = (idx_t)(cumulative - count_len_uint128(L));
    idx_t m = n - cumulative_prev - 1;        /* 0‑based index inside length L */

    /* Unrank the number of length L */
    seq_t value = 0;                          /* accumulator for the result   */
    unsigned int zeros_needed = 2;            /* zeros still to place        */
    for (unsigned int pos = 1; pos <= L; ++pos) {
        int d;                                /* current digit to try         */
        if (pos == 1) {                       /* first digit cannot be 0      */
            const int options[2] = {1, 2};
            for (int o = 0; o < 2; ++o) {
                d = options[o];
                unsigned __int128 cnt = count_strings(L - pos,
                                                      zeros_needed);
                if ((idx_t)cnt > m) {
                    value = value * (seq_t)4 + (seq_t)d;
                    break;
                } else {
                    m -= (idx_t)cnt;
                }
            }
        } else {                              /* any of 0,1,2 allowed        */
            const int options[3] = {0, 1, 2};
            for (int o = 0; o < 3; ++o) {
                d = options[o];
                unsigned int zeros_left = zeros_needed;
                if (d == 0) {
                    if (zeros_left == 0) continue;  /* cannot place more zeros */
                    zeros_left -= 1;
                }
                unsigned __int128 cnt = count_strings(L - pos,
                                                      zeros_left);
                if ((idx_t)cnt > m) {
                    value = value * (seq_t)4 + (seq_t)d;
                    zeros_needed = zeros_left;
                    break;
                } else {
                    m -= (idx_t)cnt;
                }
            }
        }
    }
    return value;
}
