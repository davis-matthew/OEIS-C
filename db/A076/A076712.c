/*
 * A076712  Triangular numbers which are also happy numbers (cf. A007770).
 * https://oeis.org/A076712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 60, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 826 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the nth triangular number that is also a happy number.
   The algorithm enumerates triangular numbers T_k = k(k+1)/2
   and checks whether each one is happy by iteratively summing
   the squares of its decimal digits until 1 or a loop is found.
   The implementation uses only integer arithmetic and works
   for any seq_t size provided by the build. */
#include <stdbool.h>

static bool is_happy(seq_t n) {
    /* A few hundred distinct values are enough to detect cycles. */
    seq_t seen[100];
    int seen_len = 0;

    while (n != 1) {
        /* check for a previously seen value → cycle */
        for (int i = 0; i < seen_len; ++i) {
            if (seen[i] == n) return false;
        }
        seen[seen_len++] = n;

        /* compute sum of squares of decimal digits */
        seq_t sum = 0;
        seq_t tmp = n;
        while (tmp != 0) {
            seq_t d = tmp % 10;
            sum += d * d;
            tmp /= 10;
        }
        n = sum;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;

    idx_t count = 0;
    seq_t k = 0;
    seq_t result = 0;

    while (count < n) {
        ++k;
        result = (seq_t)k * ((seq_t)k + 1) / 2;
        if (is_happy(result)) {
            ++count;
        }
    }

    return result;
}
