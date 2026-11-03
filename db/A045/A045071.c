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
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
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

static unsigned __int128 pow2_uint128(unsigned int e) {
    return (unsigned __int128)1 << e;
}

/* Count of valid numbers of length L (first digit non‑zero) with exactly
 * two zeros. Formula: 2^(L‑2) * C(L‑1,2). */
static unsigned __int128 count_len_uint128(unsigned int L)
{
    unsigned int rem = L - 1;          /* positions after the first digit   */
    unsigned __int128 comb = (unsigned __int128)rem * (rem - 1) / 2;
    return comb * pow2_uint128(L - 2);
}

/* Count of strings of length rem that contain exactly zeros_needed zeros
 * (each position can be 0,1,2; zeros_needed ≤ 2). */
static unsigned __int128 count_strings(unsigned int rem,
                                       unsigned int zeros_needed)
{
    if (zeros_needed > rem) return 0;
    if (zeros_needed == 0) return pow2_uint128(rem);          /* 2^rem   */
    if (zeros_needed == 1) return rem * pow2_uint128(rem-1);   /* rem·2^(rem‑1) */
    /* zeros_needed == 2 */
    return ((unsigned __int128)(rem) * (rem - 1) / 2) * pow2_uint128(rem - 2);
}

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
