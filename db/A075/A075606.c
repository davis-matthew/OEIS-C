/*
 * A075606  Smallest k not a palindrome and not divisible by 10 such that k
 *          and R(k) both are divisible by n, or 0 if n is divisible by 10.
 * https://oeis.org/A075606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A075606: a(n) = smallest k not a palindrome and not divisible by 10
 * such that k % n == 0 and reverse(k) % n == 0.
 * If n % 10 == 0, a(n) = 0. */
#include <stdbool.h>

static seq_t reverse_seq_t(seq_t x) {
    seq_t rev = 0;
    while (x > 0) {
        seq_t digit = x % 10;
        rev = rev * 10 + digit;
        x = x / 10;
    }
    return rev;
}

seq_t a(idx_t n) {
    if (n % 10 == 0) return 0;
    for (idx_t q = 1;; ++q) {
        seq_t k = (seq_t)n * q;
        if ((k % 10) == 0) continue;          /* skip multiples of 10 */
        seq_t rev = reverse_seq_t(k);
        if (rev == k) continue;               /* skip palindromes */
        if (rev % n == 0) return k;           /* found the smallest k */
    }
}
