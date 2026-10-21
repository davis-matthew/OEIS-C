/*
 * A050785  Palindromes containing at least one pair of consecutive equal
 *          digits.
 * https://oeis.org/A050785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = nth palindrome containing at least one pair of
 *            consecutive equal digits
 * Range:     u8 n <= 11, u16 n <= 208, u32 n <= 9275, u64 n <= 9275,
 *            u128 n <= 9275, big any n
 * Helpers:   has_adjacency
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_adjacency.h"

/* Helper: reverse decimal digits of a seq_t value. */
static inline seq_t reverse_num(seq_t x) {
    seq_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + (x % 10);
        x /= 10;
    }
    return rev;
}

/* Helper: construct a palindrome of given length from its left half. */
static inline seq_t make_pal(seq_t half, int len) {
    int k = (len + 1) / 2;          /* number of digits in 'half'                     */
    seq_t pow_even = 1, pow_odd = 1; /* 10^k   for even length, 10^(k-1) for odd length  */
    for (int i = 0; i < k; ++i) pow_even *= 10;
    for (int i = 0; i < k - 1; ++i) pow_odd *= 10;

    if (len % 2 == 0) { /* even length: palindrome = half || reverse(half) */
        return half * pow_even + reverse_num(half);
    } else {           /* odd  length: palindrome = half || reverse(half/10) */
        seq_t mid_removed = half / 10;
        return half * pow_odd + reverse_num(mid_removed);
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n < 1; safe fallback */

    idx_t count = 0;

    for (int len = 2; ; ++len) {          /* start at length 2 */
        int k = (len + 1) / 2;            /* digits in the left half            */

        /* left half ranges from 10^(k-1) to 10^k - 1 */
        unsigned long long start = 1;
        for (int i = 1; i < k; ++i) start *= 10ULL;
        unsigned long long end = start * 10ULL - 1ULL;

        for (unsigned long long half = start; half <= end; ++half) {
            seq_t pal = make_pal((seq_t)half, len);
            if (has_adjacency(pal)) {
                ++count;
                if (count == n) return pal;
            }
        }
    }
    /* unreachable; return 0 to satisfy compiler */
    return 0;
}
