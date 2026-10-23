/*
 * A045169  Numbers whose base-5 representation contains exactly three 0's and
 *          no 1's.
 * https://oeis.org/A045169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   comb_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the nth integer whose base‑5 representation
 * contains exactly three 0`s and no 1`s. The sequence is indexed
 * starting at 1 (OEIS offset 1). */
#include <stdint.h>
#include "../utils/comb_int.h"

/* powers of 3 up to 20 (enough for lengths needed with n<=10000). */
static const seq_t pow3[21] = {
    1ULL, 3ULL, 9ULL, 27ULL, 81ULL, 243ULL, 729ULL, 2187ULL,
    6561ULL, 19683ULL, 59049ULL, 177147ULL, 531441ULL,
    1594323ULL, 4782969ULL, 14348907ULL, 43046721ULL,
    129140163ULL, 387420489ULL, 1162261467ULL, 3486784401ULL
};

/* count how many base‑5 numbers of length L satisfy the condition. */
static seq_t countLen(int L)
{
    if (L < 4) return 0;                          /* need at least 4 digits */
    seq_t comb = comb_int(L - 1, 3);              /* choose zero positions (not first) */
    seq_t p3   = pow3[L - 3];                     /* 3^(L-3) choices for other digits */
    return comb * p3;
}

/* main function required by the framework. */
seq_t a(idx_t n)
{
    /* Find the length L such that the cumulative count reaches n. */
    seq_t cum = 0;
    int L = 4;
    seq_t cnt;
    while (1) {
        cnt = countLen(L);
        if (cum + cnt >= n) break;
        cum += cnt;
        ++L;
    }

    /* Rank of the number among those of length L (1‑based). */
    seq_t rank = n - cum;          /* rank in [1, cnt] */

    seq_t value = 0;
    int zerosUsed = 0;

    for (int pos = 1; pos <= L; ++pos) {
        /* Determine allowed digits for this position, sorted ascending. */
        const int allowed[4] = {0, 2, 3, 4};    /* 0 may appear except at leading */
        int startIdx = (pos == 1) ? 1 : 0;     /* skip 0 at position 1 */

        /* Try each digit and see how many completions it permits. */
        for (int di = startIdx; di < 4; ++di) {
            int digit = allowed[di];
            int newZeros = zerosUsed + (digit == 0 ? 1 : 0);
            int rem = L - pos;
            int needZeros = 3 - newZeros;
            seq_t ways;

            if (needZeros < 0 || needZeros > rem) {
                ways = 0;
            } else {
                seq_t c = comb_int(rem, needZeros);
                seq_t p = pow3[rem - needZeros];
                ways = c * p;
            }

            if (rank > ways) {
                rank -= ways;
            } else {
                /* choose this digit */
                value = value * 5 + digit;
                zerosUsed = newZeros;
                break;
            }
        }
    }
    return value;
}
