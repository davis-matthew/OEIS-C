/*
 * A098947  Numbers where 5 is the only odd decimal digit.
 * https://oeis.org/A098947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098947 – Numbers where 5 is the only odd decimal digit.
   The sequence lists all positive integers whose decimal digits belong
   to {0,2,4,5,6,8} and contain at least one 5, in increasing order.
   This function returns the n‑th term (a(1)=5) by combinatorial
   counting. */
#include <stdbool.h>

/* Fast exponentiation for seq_t bases. */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Number of suffixes of length 'len' that can follow the already chosen
   digits and still guarantee at least one 5 in the whole number. */
static seq_t suffix_count(idx_t len, bool has5) {
    if (len == 0)
        return has5 ? (seq_t)1 : (seq_t)0;
    seq_t total = pow_seq((seq_t)6, len);          /* all allowed digits */
    if (has5)
        return total;
    seq_t no5 = pow_seq((seq_t)5, len);            /* digits not 5 */
    return total - no5;                            /* must contain a 5 */
}

/* Allowed digits for the first position (no leading zero). */
static const int first_digits[5] = {2,4,5,6,8};
/* Allowed digits for all other positions. */
static const int all_digits[6]  = {0,2,4,5,6,8};

seq_t a(idx_t n) {
    /* 1. Find the length L of the desired number. */
    seq_t cumulative = (seq_t)0;
    idx_t len = 1;
    seq_t with5;
    while (1) {
        seq_t part1 = pow_seq((seq_t)6, len - 1);          /* 6^(len-1) */
        seq_t part2 = pow_seq((seq_t)5, len - 1);          /* 5^(len-1) */
        with5 = (seq_t)5 * part1 - (seq_t)4 * part2;       /* with at least one 5 */
        if (cumulative + with5 >= (seq_t)n) {
            break;
        }
        cumulative += with5;
        ++len;
    }
    idx_t L = len;

    /* 2. Position within numbers of length L. */
    seq_t k = (seq_t)n - cumulative;          /* 1‑based index in this length class */

    /* 3. Construct the number digit by digit. */
    seq_t result = (seq_t)0;
    bool has5 = false;

    for (idx_t pos = 1; pos <= L; ++pos) {
        const int *digs = (pos == 1) ? first_digits : all_digits;
        int sz = (pos == 1) ? 5 : 6;
        for (int i = 0; i < sz; ++i) {
            int d = digs[i];
            bool next_has5 = has5 || (d == 5);
            seq_t cnt = suffix_count(L - pos, next_has5);
            if (k > cnt) {
                k -= cnt;
            } else {
                result = result * (seq_t)10 + (seq_t)d;
                has5 = next_has5;
                break;
            }
        }
    }
    return result;
}
