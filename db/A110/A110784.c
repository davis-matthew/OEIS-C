/*
 * A110784  Palindromes in which the digits are in nondecreasing order halfway
 *          through.
 * https://oeis.org/A110784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 253, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the nth palindrome whose first half digits are
 * nondecreasing (palindromic hill numbers).  The sequence starts
 * with 1,2,...,9,11,22,...,99,111,121,131,...
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t binom_table[21][21] = {0};
static bool binom_inited = false;

static void init_binom(void)
{
    if (binom_inited) return;
    for (int i = 0; i <= 20; ++i) {
        binom_table[i][0] = binom_table[i][i] = 1;
        for (int j = 1; j < i; ++j) {
            binom_table[i][j] = binom_table[i-1][j-1] + binom_table[i-1][j];
        }
    }
    binom_inited = true;
}

/* number of nondecreasing sequences of length m whose digits are in
 * the range [min_digit, 9] (min_digit >= 1) */
static seq_t count_nondec(idx_t m, int min_digit)
{
    if (m == 0) return 1;
    int alphabet = 10 - min_digit;          /* digits min_digit .. 9    */
    int n = alphabet + m - 1;               /* choose m from n values  */
    return binom_table[n][m];
}

/* number of palindromes of length L with nondecreasing first half */
static seq_t count_len(idx_t L)
{
    if (L % 2 == 0) {                    /* even length */
        int k = L / 2;
        return binom_table[8 + k][k];
    } else {                              /* odd length  */
        int k = (L - 1) / 2;
        int M = k + 1;
        return binom_table[8 + M][M];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset 1 is defined */
    init_binom();

    idx_t remaining = n;
    idx_t L = 1;
    while (1) {
        seq_t cnt = count_len(L);
        if (remaining <= (idx_t)cnt) break;
        remaining -= (idx_t)cnt;
        ++L;
    }

    idx_t M = (L + 1) / 2;                /* number of free digits   */
    idx_t rank = remaining - 1;           /* 0‑based index in this len */

    int digits[12] = {0};                 /* max needed for length 12 */
    int min_digit = 1;

    for (idx_t pos = 0; pos < M; ++pos) {
        int rem = M - pos - 1;            /* positions left after this one */
        for (int d = min_digit; d <= 9; ++d) {
            seq_t c = count_nondec(rem, d);
            if (rank < (idx_t)c) {
                digits[pos] = d;
                min_digit = d;
                break;
            }
            rank -= (idx_t)c;
        }
    }

    seq_t val = 0;
    for (idx_t pos = 0; pos < M; ++pos)
        val = val * 10 + digits[pos];

    int start = (int)((L & 1) ? M - 2 : M - 1); /* mirror start index */
    for (int pos = start; pos >= 0; --pos)
        val = val * 10 + digits[pos];

    return val;
}
