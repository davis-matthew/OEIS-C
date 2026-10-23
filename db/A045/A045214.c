/*
 * A045214  Numbers whose base-5 representation contains exactly three 0's and
 *          no 4's.
 * https://oeis.org/A045214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose base‑5 representation *
 *            contains exactly three 0's and no 4's, listed in increasing
 *            order. * Each term a(n) is the number whose base‑5 digits
 *            consist of the * symbols {0,1,2,3} with the constraint that
 *            exactly three of them are 0
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Small integer helpers                                               */
/* --------------------------------------------------------------------- */

/* Binomial coefficient C(n,k) for uint64_t, using a 128‑bit intermediate
 * to avoid overflow during multiplication.  Returns 0 if k>n. */
static idx_t binom_u64(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        __int128 tmp = (__int128)res * (n - k + i) / i;
        res = (idx_t)tmp;
    }
    return res;
}

/* Compute 3^e as idx_t; if the value does not fit in idx_t, return
 * the maximum value of idx_t (used only for small e, so this never
 * happens in the ranges of interest). */
static idx_t pow3_u64(idx_t e)
{
    __int128 r = 1;
    while (e--) r *= 3;
    /* safe for the problem sizes */
    return (idx_t)r;
}

/* Count completions for the remaining positions:
 *   positions   – number of free slots left,
 *   zeros_left  – how many zeros still have to be placed.
 *
 * For each of the remaining positions we may choose either 0 (if we
 * still need zeros) or one of {1,2,3}.  The number of ways to choose
 * the zero positions is C(positions, zeros_left), and the other
 * positions each have 3 possibilities. */
static idx_t count_completions(idx_t positions, idx_t zeros_left)
{
    if (zeros_left > positions) return 0;
    idx_t comb = binom_u64(positions, zeros_left);
    idx_t pow_part = pow3_u64(positions - zeros_left);
    __int128 prod = (__int128)comb * pow_part;
    return (idx_t)prod;
}

/* --------------------------------------------------------------------- */
/*  Sequence function                                                   */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Find the base‑5 length L of the n‑th number.  Numbers of length L
     * are counted as:
     *   C(L-1,3) * 3^(L-3)        (zeros placed in positions 1..L-1,
     *   leading digit chosen from {1,2,3} afterwards). */
    idx_t cumulative = 0;
    idx_t L = 4;                /* minimal length: 3 zeros + 1 non‑zero */
    while (1) {
        idx_t cnt_len = binom_u64(L - 1, 3) * pow3_u64(L - 3);
        if (cumulative + cnt_len >= n) break;
        cumulative += cnt_len;
        ++L;
    }

    /* Rank of n within numbers of length L (0‑based). */
    idx_t rank_len = n - cumulative - 1;

    /* Size of each leading‑digit block: C(L-1,3)*3^(L-4) */
    idx_t group_size = binom_u64(L - 1, 3) * pow3_u64(L - 4);

    /* Determine which leading digit (1,2,3) is used. */
    idx_t leading_idx = rank_len / group_size;      /* 0,1,2 */
    int leading_digit = (int)(leading_idx + 1);

    /* Rank within that leading‑digit block. */
    idx_t rank = rank_len % group_size;

    /* Array to hold the base‑5 digits.  Size big enough for all lengths
     * considered (L<=8 for n up to 10000, but we keep it safe). */
    int digits[64];
    digits[0] = leading_digit;

    /* Remaining zeros to place among positions 1..L-1. */
    idx_t zeros_left = 3;
    for (idx_t pos = 1; pos < L; ++pos) {
        idx_t rem_pos_after = L - 1 - pos;  /* positions after current */

        for (int d = 0; d <= 3; ++d) {
            if (d == 0 && zeros_left == 0) continue;     /* no zeros left */

            idx_t zeros_next = zeros_left - (d == 0 ? 1 : 0);
            idx_t cnt = count_completions(rem_pos_after, zeros_next);

            if (cnt > rank) {
                digits[pos] = d;
                zeros_left = zeros_next;
                break;
            } else {
                rank -= cnt;
            }
        }
    }

    /* Convert the base‑5 digits to a decimal number.  The arithmetic is
     * performed in the seq_t type; seq_t supports * and + with integer
     * constants. */
    seq_t value = 0;
    for (idx_t i = 0; i < L; ++i) {
        value = value * 5 + (seq_t)digits[i];
    }
    return value;
}
