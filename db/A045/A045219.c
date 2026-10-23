/*
 * A045219  Numbers whose base-5 representation contains no 1's and exactly
 *          two 2's.
 * https://oeis.org/A045219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045219: Numbers whose base‑5 representation contains
 * no 1’s and exactly two 2’s.
 * 
 * The algorithm counts how many numbers exist with a given number of
 * digits, then for the required rank n determines the digit sequence
 * by combinatorial unranking.
 * 
 * Count for length L (L ≥ 2):
 *   count(L) = (L‑1) * 3^(L‑3) * (L+1)    for L ≥ 3
 *   count(2) = 1
 *   (derived from choosing positions for the two 2's and filling
 *    remaining positions with digits {0,3,4}).
 *
 *  The unranking loops over each digit position, testing the
 *  admissible digits in increasing order and using combinatorial
 *  counts to skip whole blocks.  The result is assembled in base 5.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: 3^exp as unsigned 128‑bit */
static inline unsigned __int128 pow3(unsigned int exp) {
    unsigned __int128 r = 1;
    for (unsigned int i = 0; i < exp; ++i) r *= 3;
    return r;
}

/* Helper: count of strings of length len with exactly t twos,
 * with leading zeros allowed.  t ∈ {0,1,2}. */
static inline unsigned __int128 count_rest(unsigned int len, int t) {
    if (t < 0 || t > (int)len) return 0;
    if (t == 0) return pow3(len);
    if (t == 1) return (unsigned __int128)len * pow3(len - 1);
    /* t == 2 */
    return (unsigned __int128)len * (len - 1) / 2 * pow3(len - 2);
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Find the length L such that the first n numbers lie within lengths ≤ L */
    unsigned __int128 cum = 0;
    unsigned int L = 2;
    while (1) {
        unsigned __int128 cnt;
        if (L == 2) cnt = 1;
        else        cnt = (unsigned __int128)(L - 1) * (L + 1) * pow3(L - 3);
        if (cum + cnt >= (unsigned __int128)n) break;
        cum += cnt;
        ++L;
    }

    /* Rank within length-L block (0‑based) */
    unsigned __int128 k = (unsigned __int128)n - cum - 1;

    /* Unrank the digit sequence */
    seq_t val = 0;
    unsigned int rem = L;   /* positions left to decide */
    int rem2 = 2;           /* number of 2's still to place */

    for (unsigned int pos = 0; pos < L; ++pos) {
        /* Candidate digits in ascending order; 0 forbidden at pos 0 */
        int cand[4] = {0, 2, 3, 4};
        for (int ci = 0; ci < 4; ++ci) {
            int d = cand[ci];
            if (pos == 0 && d == 0) continue;

            unsigned int m = rem - 1;
            int tprime = rem2 - (d == 2 ? 1 : 0);
            unsigned __int128 cnt = count_rest(m, tprime);

            if (k >= cnt) {
                k -= cnt;
                continue;
            }

            /* Pick this digit */
            val = val * 5 + (seq_t)d;
            rem = m;
            rem2 = tprime;
            break;
        }
    }
    return val;
}
