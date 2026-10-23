/*
 * A045149  Numbers whose base-4 representation contains exactly three 2's and
 *          one 3.
 * https://oeis.org/A045149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑4 representation contains exactly three 2's and one 3.
   This function returns the nth term (offset 1) by constructing the
   base‑4 representation digit by digit and counting how many numbers
   start with each possible prefix. */
#include "../utils/binom.h"
#include "../utils/pow2.h"

/* seq_t is the build‑specific unsigned integer type;
   idx_t is a 64‑bit unsigned index type. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset 1 – never used */

    /* Determine the length L of the base‑4 representation that contains
       the nth number.  For length L ≥4 the number of valid numbers is   */
    /*   4 * C(L,4) * 2^(L-4)  –  4 * C(L-1,4) * 2^(L-5)   (msd ≠ 0).   */
    idx_t sum = 0;                      /* count of numbers with length < L */
    idx_t L = 4;                         /* minimal possible length */

    for (;;) {
        /* count(L) as seq_t (always small enough to fit into idx_t)   */
        seq_t cnt_len = 0;

        /* first term: 4 * C(L,4) * 2^(L-4) */
        seq_t term1 = binom(L, 4);
        term1 *= pow2(L - 4);
        term1 *= (seq_t)4;

        /* second term: 0 for L=4; otherwise 4 * C(L-1,4) * 2^(L-5) */
        seq_t term2 = 0;
        if (L >= 5) {
            term2 = binom(L - 1, 4);
            term2 *= pow2(L - 5);
            term2 *= (seq_t)4;
        }

        cnt_len = term1 - term2;       /* non‑negative for L ≥ 4 */

        idx_t cnt_len_idx = (idx_t)cnt_len;
        if (sum + cnt_len_idx >= n) break;
        sum += cnt_len_idx;
        ++L;
    }

    /* index of the desired number within the current length (1‑based) */
    idx_t n0 = n - sum;

    /* store the digits (index ==> position, 0 … L-1) */
    int digits[64] = {0};

    /* remaining counts of 2's and 3's */
    int r2 = 3, r3 = 1;

    /* construct the number from the most significant digit downwards */
    for (idx_t pos = L; pos > 0; ) {
        pos--;                          /* current position (0 … L-1) */
        int chosen = -1;

        /* possible digits for this position */
        int cand[4], ncand;
        if (pos == L - 1) {             /* most significant digit */
            cand[0] = 1; cand[1] = 2; cand[2] = 3;
            ncand = 3;
        } else {
            cand[0] = 0; cand[1] = 1; cand[2] = 2; cand[3] = 3;
            ncand = 4;
        }

        /* try each candidate in increasing order */
        for (int ci = 0; ci < ncand; ++ci) {
            int d = cand[ci];

            /* skip impossible usage of 2 or 3 */
            if ((d == 2) && (r2 == 0)) continue;
            if ((d == 3) && (r3 == 0)) continue;

            int new_r2 = r2 - (d == 2);
            int new_r3 = r3 - (d == 3);

            /* remaining positions below this one */
            idx_t rem_pos = pos;

            /* check feasibility: enough free slots for the remaining 2's & 3's */
            if (rem_pos < (idx_t)(new_r2 + new_r3)) continue;

            /* number of completions for this prefix:
               choose places for the remaining 3's, then 2's,
               and fill the rest with 0 or 1 (two choices each).        */
            seq_t c1 = binom(rem_pos, (idx_t)new_r3);
            seq_t c2 = binom(rem_pos - new_r3, (idx_t)new_r2);
            seq_t c3 = pow2(rem_pos - new_r2 - new_r3);

            seq_t cnt_seq = c1 * c2 * c3;
            idx_t cnt = (idx_t)cnt_seq;

            if (n0 > cnt) {
                n0 -= cnt;               /* skip this whole block */
                continue;
            } else {                    /* the desired number starts with d */
                chosen = d;
                r2 = new_r2;
                r3 = new_r3;
                break;
            }
        }

        /* chosen must have been found */
        digits[pos] = chosen;
    }

    /* convert the digit array to the numeric value */
    seq_t value = 0;
    for (idx_t pos = L; pos > 0; ) {
        pos--;
        value = value * 4 + (seq_t)digits[pos];
    }

    return value;
}
