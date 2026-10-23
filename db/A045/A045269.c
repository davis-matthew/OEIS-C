/*
 * A045269  Numbers whose base-5 representation contains exactly one 2 and
 *          three 3's.
 * https://oeis.org/A045269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑5 representation contains exactly one digit 2
 *            and three digits 3
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Small binomial coefficient C(n,k).  k is ≤ 4. */
static inline seq_t binomial(seq_t n, seq_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (seq_t i = 1; i <= k; ++i) {
        result = result * (n - k + i);
        result = result / i;   /* exact division */
    }
    return result;
}

/* Power of 3: 3^exp   (exp ≥ 0).  exp fits in size_t. */
static inline seq_t pow3(size_t exp)
{
    seq_t r = 1;
    while (exp-- > 0) r *= 3;
    return r;
}

/* Number of ways to fill `rem` remaining positions with
   `twos` remaining 2's and `threes` remaining 3's.
   Remaining positions may contain 0,1,4 (3 choices each). */
static inline seq_t count_completions(seq_t rem, seq_t twos, seq_t threes)
{
    if (twos + threes > rem) return 0;
    seq_t ways_twos   = binomial(rem, twos);
    seq_t ways_threes = binomial(rem - twos, threes);
    seq_t ways_others = 1;
    seq_t exp = rem - twos - threes;
    while (exp > 0) {
        ways_others *= 3;
        exp -= 1;
    }
    return ways_twos * ways_threes * ways_others;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* 0‑based index within the sequence. */
    seq_t idx = (seq_t)(n - 1);   /* n ≥ 1 */

    /* Find the base‑5 length of the required number. */
    size_t length = 4;            /* minimal length */
    for (;;) {
        seq_t total_all = binomial((seq_t)length, (seq_t)4) * 4 * pow3(length - 4);
        seq_t bad_leading = 0;
        if (length > 4) {
            bad_leading = binomial((seq_t)length - 1, (seq_t)4) * 4 * pow3(length - 5);
        }
        seq_t count_len = total_all - bad_leading;
        if (idx < count_len) break;
        idx -= count_len;
        ++length;
    }

    /* Construct the number digit by digit. */
    seq_t two_left   = 1;   /* one '2' still to place */
    seq_t three_left = 3;   /* three '3's still to place */
    seq_t value = 0;        /* resulting number */

    for (size_t pos = 0; pos < length; ++pos) {
        seq_t rem = (seq_t)(length - pos - 1);
        int min_digit = (pos == 0) ? 1 : 0;   /* leading digit ≠ 0 */

        for (int d = min_digit; d <= 4; ++d) {
            if (d == 2 && two_left == 0) continue;
            if (d == 3 && three_left == 0) continue;

            seq_t twos   = two_left;
            seq_t threes = three_left;
            if (d == 2) twos   -= 1;
            if (d == 3) threes -= 1;

            seq_t cnt = count_completions(rem, twos, threes);
            if (idx >= cnt) {
                idx -= cnt;
                continue;
            }

            /* Choose this digit */
            value = value * 5 + (seq_t)d;
            if (d == 2) two_left -= 1;
            if (d == 3) three_left -= 1;
            break;
        }
    }

    return value;
}
