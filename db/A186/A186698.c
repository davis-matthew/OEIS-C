/*
 * A186698  Next prime after n-th positive palindrome.
 * https://oeis.org/A186698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 753, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes a(n) = the next prime greater than the n-th positive
 * palindrome.  The nth palindrome is generated directly and the
 * next prime is found by simple trial division (fast enough for the
 * n ≤ 10000 hard‑limit of the problem).
 */
#include <stdint.h>
#include <stdbool.h>

/* Return the n-th positive palindrome (1‑based). */
static seq_t nth_palindrome(idx_t n)
{
    idx_t cumulative = 0;
    for (idx_t len = 1;; ++len) {
        /* number of digits in the left half (including middle if odd) */
        idx_t half = (len + 1) / 2;
        /* smallest half: 10^(half-1) */
        idx_t start = 1;
        for (idx_t i = 1; i < half; ++i) start *= 10;
        idx_t count_len = 9 * start;          /* palindromes of this length */

        if (cumulative + count_len >= n) {
            idx_t offset = n - cumulative - 1;           /* 0‑based */
            seq_t prefix = (seq_t)start + (seq_t)offset; /* left half */
            seq_t pal = prefix;

            /* reflect the left half (skip the middle digit if odd length) */
            seq_t tmp = prefix;
            if (len % 2 == 1) tmp /= 10;
            while (tmp > 0) {
                pal = pal * 10 + (tmp % 10);
                tmp /= 10;
            }
            return pal;
        }
        cumulative += count_len;
    }
}

/* Simple primality test for numbers up to about 10^7 (used for n ≤ 10000). */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t p = nth_palindrome(n);
    uint64_t start = (uint64_t)(p + 1);   /* start searching after the palindrome */
    uint64_t candidate = start;

    while (!is_prime_uint64(candidate))
        ++candidate;

    return (seq_t)candidate;
}
