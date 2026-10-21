/*
 * A098942  Numbers where 4 is the only even decimal digit.
 * https://oeis.org/A098942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers where 4 is the only even decimal digit
 * a(1) = 4, a(2) = 14, a(3) = 34, a(4) = 41, ...
 *
 * The algorithm:
 *   1. Compute the digit length L of the n‑th term by subtracting
 *      the cumulative counts for numbers with fewer digits.
 *   2. For the numbers of length L, count how many satisfy the
 *      "at least one 4" constraint for each possible prefix.
 *   3. Build the n‑th number digit by digit in lexicographic order.
 *      The allowed digits are 1,3,4,5,7,9.
 *   4. If the digit chosen is 4 we relax the constraint for the
 *      remaining positions (any combination is allowed); otherwise
 *      we must still place at least one 4 in the suffix.
 *
 * Since the n required is modest (≤ 10000), the maximum length is 6
 * and all intermediate powers (6^k, 5^k) fit comfortably into
 * seq_t (even 64‑bit). 
 */

#include <stdbool.h>

static const seq_t pow5[7] = {
    /* 5^0, 5^1, ..., 5^6 */
    (seq_t)1, (seq_t)5, (seq_t)25, (seq_t)125,
    (seq_t)625, (seq_t)3125, (seq_t)15625
};

static const seq_t pow6[7] = {
    /* 6^0, 6^1, ..., 6^6 */
    (seq_t)1, (seq_t)6, (seq_t)36, (seq_t)216,
    (seq_t)1296, (seq_t)7776, (seq_t)46656
};

/* The set of allowed digits in ascending order */
static const unsigned char ALLOWED[6] = {1, 3, 4, 5, 7, 9};

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of range, safe guard */

    /* 1. Determine length L of the n-th number */
    idx_t len = 1;
    idx_t cumulative = 0;            /* terms with length < len */
    while (1) {
        idx_t count_len = (idx_t)(pow6[len] - pow5[len]); /* 6^len - 5^len */
        if (cumulative + count_len >= n) break;
        cumulative += count_len;
        ++len;
    }

    /* Rank of the term within numbers of this length */
    seq_t k = (seq_t)(n - cumulative);   /* 1‑based index within this block */

    seq_t value = 0;
    bool has_four = false;

    for (idx_t pos = 1; pos <= len; ++pos) {
        idx_t rem = len - pos;     /* remaining positions after this one */

        for (idx_t i = 0; i < 6; ++i) {
            unsigned char d = ALLOWED[i];
            bool new_has_four = has_four || (d == 4);

            seq_t count;
            if (new_has_four) {
                count = pow6[rem];                          /* any suffix */
            } else {
                count = pow6[rem] - pow5[rem];              /* suffix must contain a 4 */
            }

            if (k > count) {
                k -= count;   /* skip this block */
                continue;
            }
            /* choose this digit */
            value = value * (seq_t)10 + (seq_t)d;
            has_four = new_has_four;
            break;
        }
    }
    return value;
}
