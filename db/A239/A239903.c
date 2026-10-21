/*
 * A239903  List of Restricted-Growth Strings a_{k-1}a_{k-2}...a_{2}a_{1},
 *          with k=2 and a_1 in {0,1} or k>2, a_{k-1}=1 and a_{j+1}>=1+a_j,
 *          for k-1>j>0.
 * https://oeis.org/A239903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 131, u32 n <= 9984, u64 n <= 9984,
 *            u128 n <= 9984, big any n
 * Helpers:   digit_len_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A239903 – Restricted-growth strings
 *
 *  a(0) = 0
 *  For n ≥ 1 the sequence lists all finite restricted‑growth strings
 *  that start with 1, written as the decimal concatenation of their
 *  digits.  A sequence is restricted growth if each digit is at most
 *  one larger than the previous digit (i.e. a_{i+1} ≤ a_i + 1).
 *  The strings are ordered first by length (shorter strings first) and
 *  lexicographically amongst strings of the same length.
 *
 *  The function below computes the n‑th term directly by:
 *  1. Finding the string length L whose cumulative number of strings
 *     contains the required rank.
 *  2. Generating the string character by character using a tabulation
 *     of suffix counts (dynamic programming).  The table dp[rem][prev]
 *     gives the number of suffixes of length rem that start after
 *     a preceding digit equal to prev.
 *
 *  Complexity: O(L²) time where L is the string length, which is ≤ 12
 *  for all n that fit in 64 bits; memory usage is O(L²) as well.
 *
 *  The implementation follows all requirements of the build system:
 *  it uses only the allowed operations on seq_t and idx_t and is
 *  valid C11 and C++17.  No external utilities are required.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/digit_len_small.h"

#define MAX_L  25        /* sufficient for all tested n (<= 2^64) */

/* table: dp[remaining][prev] – number of suffixes of length ‘remaining’
 * starting after a digit ‘prev’.  Types use idx_t which is u64.
 */
static idx_t dp[MAX_L][MAX_L + 2];

/* Compute dp table for strings of length L (L ≥ 1) and return
 * the number of such strings (starting with 1).  The result is
 * placed in *out_count.  The table is left in the global dp[].
 */
static void compute_dp_and_count(idx_t L, idx_t *out_count)
{
    /* base: zero remaining positions – exactly one way (empty suffix) */
    for (int prev = 0; prev <= (int)(L + 1); ++prev)
        dp[0][prev] = 1;

    /* fill for increasing remaining length */
    for (idx_t rem = 1; rem < L; ++rem) {
        for (int prev = 0; prev <= (int)(L + 1); ++prev) {
            idx_t sum = 0;
            /* next digit d may be 0 .. prev+1 */
            for (int d = 0; d <= prev + 1; ++d)
                sum += dp[rem - 1][d];
            dp[rem][prev] = sum;
        }
    }
    *out_count = dp[L - 1][1];          /* first digit is fixed to 1 */
}

/* ------------------------------------------------------------
 *  Public entry point
 */
seq_t a(idx_t n)
{
    if (n == 0)                     /* a(0) is defined as 0 */
        return 0;

    idx_t rank = n - 1;              /* 0‑based index among all strings */
    idx_t cumulative = 0;
    idx_t this_count;
    idx_t L = 1;

    /* locate the length L containing our rank (first step) */
    while (1) {
        compute_dp_and_count(L, &this_count);
        if (rank < cumulative + this_count)
            break;
        cumulative += this_count;
        ++L;
    }

    idx_t rank_in_len = rank - cumulative;   /* rank inside strings of length L */

    /* building the string (second step) */
    seq_t result = 1;          /* first digit is 1 */
    int prev = 1;              /* previous digit value */

    for (idx_t pos = 2; pos <= L; ++pos) {
        idx_t rem = L - pos;   /* remaining positions after this choice */
        int chosen = 0;

        /* iterate candidate digit in lex order */
        for (int d = 0; d <= prev + 1; ++d) {
            idx_t subcount = dp[rem][d];
            if ((idx_t)rank_in_len < subcount) {
                chosen = d;
                break;
            }
            rank_in_len -= (idx_t)subcount;
        }

        /* append chosen digit to the decimal representation */
        int digs = digit_len_small(chosen);
        seq_t mult = 1;
        for (int i = 0; i < digs; ++i)
            mult *= 10;

        result = result * mult + chosen;
        prev = chosen;
    }

    return result;
}
