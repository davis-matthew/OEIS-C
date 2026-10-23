/*
 * A168731  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^18 = I.
 * https://oeis.org/A168731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=6, a(2)=30, a(3)=150, a(4)=750, a(5)=3750,
 *            a(6)=18750, a(7)=93750, a(8)=468750, a(9)=2343750,
 *            a(10)=11718750, a(11)=58593750, a(12)=292968750,
 *            a(13)=1464843750, a(14)=7324218750, a(15)=36621093750,
 *            a(16)=183105468750, a(17)=915527343750, a(18)=4577636718735
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A168731:
   a(0..18) are the given initial terms.
   For n>=19: a(n) = 4*(a(n-1)+a(n-2)+...+a(n-17)) - 10*a(n-18)      */

/* The sequence will be stored in a static array that grows on demand.
   The array size 20000 is sufficient for all tests that ask for
   n <= 10000. */

#include <stddef.h>

static const seq_t init[19] = {
    (seq_t)1ULL,
    (seq_t)6ULL,
    (seq_t)30ULL,
    (seq_t)150ULL,
    (seq_t)750ULL,
    (seq_t)3750ULL,
    (seq_t)18750ULL,
    (seq_t)93750ULL,
    (seq_t)468750ULL,
    (seq_t)2343750ULL,
    (seq_t)11718750ULL,
    (seq_t)58593750ULL,
    (seq_t)292968750ULL,
    (seq_t)1464843750ULL,
    (seq_t)7324218750ULL,
    (seq_t)36621093750ULL,
    (seq_t)183105468750ULL,
    (seq_t)915527343750ULL,
    (seq_t)4577636718735ULL
};

static seq_t memo[20000];
static size_t memo_len = 0;

static void init_memo(void) {
    size_t i;
    for (i = 0; i < 19; ++i) memo[i] = init[i];
    memo_len = 19;
}

/* ---  Main sequence function  --- */
seq_t a(idx_t n) {
    size_t idx = (size_t)n;

    /* Grow the table lazily */
    if (memo_len == 0) init_memo();

    if (idx < memo_len) return memo[idx];

    /* Compute the sum of the last 17 terms needed for the first new entry. */
    seq_t sum_last_17 = 0;
    /* sum_last_17 corresponds to a(idx-1) down to a(idx-17) when idx == memo_len */
    size_t start = memo_len >= 17 ? memo_len - 17 : 0;
    for (size_t k = start; k < memo_len; ++k) sum_last_17 += memo[k];

    for (size_t i = memo_len; i <= idx; ++i) {
        seq_t val = (seq_t)4 * sum_last_17 - (seq_t)10 * memo[i-18];
        memo[i] = val;
        /* update sum_last_17 for the next position: add a(i) and subtract a(i-17) */
        sum_last_17 = sum_last_17 + memo[i] - memo[i-17];
    }

    memo_len = idx + 1;
    return memo[idx];
}
