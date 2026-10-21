/*
 * A232668  Natural numbers that are not (primes, 11-smooth, perfect powers or
 *          base-10 palindromes).
 * https://oeis.org/A232668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n-th natural number that is NOT a prime, NOT an
 *            11‑smooth number, NOT a perfect power, and NOT a base‑10
 *            palindrome
 * Range:     u8 n <= 45, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   bitlen_seq_t, grow_seq_array, is_11smooth, is_palindrome,
 *            is_prime_big, pow2, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow_uint.h"
#include "../utils/bitlen_seq_t.h"
#include "../utils/is_palindrome.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq_array.h"
#include "../utils/pow2.h"
#include "../utils/is_11smooth.h"

/* Helper: integer k‑th root of n, floor. */
static seq_t nth_root(seq_t n, int k)
{
    seq_t low  = 1;
    seq_t high = pow2((bitlen_seq_t(n) + k - 1) / k + 1);
    seq_t ans  = 1;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        seq_t val = pow_uint((uint64_t)mid, k);
        if (val == n)
            return mid;
        if (val < n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Helper: true if x is a perfect power (m^k, m>0, k>=2). */
static bool is_perfect_power(seq_t x)
{
    if (x <= 3)  /* 2^2 = 4 is first perfect power */
        return false;

    unsigned bits = bitlen_seq_t(x);
    for (int k = 2; k <= (int)bits; ++k) {
        seq_t r = nth_root(x, k);
        if (r <= 1)
            continue;
        if (pow_uint((uint64_t)r, k) == x)
            return true;
    }
    return false;
}

/* Helper: true if x belongs to any excluded category. */
static bool is_excluded(seq_t x)
{
    if (x < 2)
        return true;                 /* 1 is a palindrome anyway */

    if (is_prime_big(x))
        return true;                 /* prime */

    if (is_11smooth(x))
        return true;                 /* 11‑smooth */

    if (is_palindrome(x, 10))
        return true;                 /* base‑10 palindrome */

    if (is_perfect_power(x))
        return true;                 /* perfect power */

    return false;
}

/* Main function implementing the sequence. */
seq_t a(idx_t n)
{
    static seq_t *seq_arr = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static seq_t next_val = 2;            /* start checking from 2 */

    while (seq_len < n) {
        if (!is_excluded(next_val)) {
            grow_seq_array(&seq_arr, &seq_cap, seq_len + 1);
            seq_arr[seq_len++] = next_val;
        }
        ++next_val;
    }
    return seq_arr[n - 1];
}
