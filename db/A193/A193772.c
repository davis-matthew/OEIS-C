/*
 * A193772  Nonnegative integers whose digital difference is 0.
 * https://oeis.org/A193772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 491, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: nonnegative integers whose first digit equals the sum of
   the remaining decimal digits.  Numbers are listed in increasing
   order, starting with 0. */

#include <stdint.h>

/* Helper: binomial coefficient C(n,k), n,k small (≤ 20) */
static seq_t comb(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (int i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* Main function required by the test harness */
seq_t a(idx_t n) {
    /* n is 1‑based; a(1) = 0 */
    if (n == 1) return 0;

    /* Find the length (number of digits) of the n‑th number */
    idx_t cumulative = 1;        /* count of numbers ≤ current length (including 0) */
    idx_t len = 2;               /* start from two‑digit numbers */

    while (1) {
        idx_t m = len - 1;        /* remaining digits after the first */
        idx_t count_len = 0;
        for (int d = 1; d <= 9; ++d) {
            count_len += (idx_t)comb(d + m - 1, m - 1);
        }
        if (cumulative + count_len >= n) break;
        cumulative += count_len;
        ++len;
    }

    /* rank within the group of numbers of length 'len' (1‑based) */
    idx_t rank = n - cumulative;

    /* Select the first digit */
    idx_t m = len - 1;               /* remaining positions after the first */
    int first_digit = 1;
    idx_t acc = 0;
    for (int d = 1; d <= 9; ++d) {
        idx_t cnt = (idx_t)comb(d + m - 1, m - 1);
        if (acc + cnt >= rank) {
            first_digit = d;
            rank -= acc;
            break;
        }
        acc += cnt;
    }

    /* Construct the remaining digits recursively */
    seq_t result = first_digit;
    int remaining_sum = first_digit;
    int positions = m;

    for (int pos = 0; pos < m; ++pos) {
        for (int val = 0; val <= 9; ++val) {
            if (val > remaining_sum) break;
            int rem_digits = positions - 1;
            int rem_sum = remaining_sum - val;
            idx_t cnt;
            if (rem_digits == 0) {
                cnt = (rem_sum == 0) ? 1 : 0;
            } else {
                cnt = (idx_t)comb(rem_sum + rem_digits - 1, rem_digits - 1);
            }
            if (cnt >= rank) {
                result = result * 10 + val;
                remaining_sum -= val;
                positions = rem_digits;
                break;
            } else {
                rank -= cnt;
            }
        }
    }

    return result;
}
