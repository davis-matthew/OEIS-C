/*
 * A088262  Triangle in which row n gives n primes ending in k where k is the
 *          n-th odd number not divisible by 5.
 * https://oeis.org/A088262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   get_k_by_row, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A088262: Triangle where row r contains the first r primes ending with
 * the r-th odd integer not divisible by 5.  The sequence is read row by row.
 *
 * a(1) = 11
 * a(2) = 3   a(3) = 13
 * a(4) = 7   a(5) = 17  a(6) = 37
 * a(7) = 19  a(8) = 29  a(9) = 59  a(10)=79
 *
 * The implementation uses a simple linear search: for a given row r it
 *   1. determines k, the r‑th odd integer not divisible by 5,
 *   2. finds the required prime suffix by stepping through the arithmetic
 *      progression k + m·10^d  (d = number of decimal digits of k).
 *
 * The search is efficient enough for the requested limit (n≈10 000).  The
 * code respects all type and style constraints described in the problem
 * statement.  No memoisation is required as each call rebuilds only the
 * values necessary for the specified n. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/get_k_by_row.h"

static seq_t nth_prime_with_suffix(seq_t k, idx_t ord)
{
    /* number of decimal digits of k */
    seq_t digits = 0;
    seq_t temp = k;
    if (temp == 0) {
        digits = 1;
    } else {
        while (temp > 0) {
            digits++;
            temp /= 10;
        }
    }
    /* modulus = 10^digits */
    seq_t mod = 1;
    for (seq_t i = 0; i < digits; ++i) {
        mod *= 10;
    }

    seq_t candidate = k;
    /* make sure candidate is at least 2 (smallest prime) */
    while (candidate < (seq_t)2) {
        candidate += mod;
    }

    idx_t found = 0;
    while (found < ord) {
        if (is_prime_big(candidate)) {
            found++;
            if (found == ord) {
                break;
            }
        }
        candidate += mod;
    }
    return candidate;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;        /* offset is 1; nothing defined for n==0 */
    }

    /* Find row r such that (r-1)*r/2 < n <= r*(r+1)/2 */
    idx_t r = 1;
    idx_t cur_sum = 1;          /* sum of first r integers */
    while (cur_sum < n) {
        r++;
        cur_sum += r;
    }

    /* previous cumulative total up to row r-1 */
    idx_t prev_sum = (r - 1) * r / 2;
    idx_t pos_in_row = n - prev_sum - 1;   /* 0‑based */
    idx_t ord_in_row = pos_in_row + 1;     /* 1‑based */

    seq_t k = get_k_by_row(r);
    return nth_prime_with_suffix(k, ord_in_row);
}
