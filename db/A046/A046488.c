/*
 * A046488  Palindromes that are the sum of the first n palindromes for some
 *          n.
 * https://oeis.org/A046488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A046488: palindromes that equal the sum of the first m palindromes
   The function generates decimal palindromes in increasing order,
   maintains a running sum of all palindromes seen,
   and counts how many of those sums are themselves decimal palindromes.
   When the counter reaches the requested index n it returns that sum. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_palindrome.h"   /* bool is_palindrome(seq_t x, idx_t base); */

/* Forward declaration of helper functions */
static seq_t make_pal(uint64_t half, bool odd);
static uint64_t pow10_uint64(int k);

/* Generate the nth palindrome sum on demand.
   No memoization is performed; each call recomputes from scratch
   but is efficient enough for n up to about 10 000. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined index 0 */

    idx_t counted = 0;             /* number of palindrome sums found */
    seq_t sum = 0;                 /* running sum of all palindromes */

    /* State for generating the next decimal palindrome in order */
    idx_t cur_len = 1;              /* current length of palindrome (digits) */
    uint64_t cur_half = 1;          /* current half value used to build palindrome */
    uint64_t half_start = 1;        /* first half value for current length */
    uint64_t half_end   = 9;        /* last half value for current length */

    while (counted < (idx_t)n) {
        /* Build next palindrome */
        bool odd = (cur_len & 1) != 0;
        seq_t pal = make_pal(cur_half, odd);

        /* Update running sum */
        sum += pal;

        /* If the sum itself is a palindrome, count it */
        if (is_palindrome(sum, 10)) {
            counted++;
            if (counted == (idx_t)n)
                return sum;
        }

        /* Advance to the next palindrome */
        cur_half++;
        if (cur_half > half_end) {
            cur_len++;
            int half_len = (cur_len + 1) / 2;                 /* length of half part */
            half_start = pow10_uint64(half_len - 1);
            half_end   = pow10_uint64(half_len) - 1u;
            cur_half   = half_start;
        }
    }
    return sum;   /* should never reach here */
}

/* Construct a palindrome from a half value.
   If odd is true the middle digit is omitted during mirroring. */
static seq_t make_pal(uint64_t half, bool odd) {
    uint64_t rev = odd ? half / 10 : half;
    uint64_t pal = half;
    while (rev > 0) {
        pal = pal * 10 + rev % 10;
        rev /= 10;
    }
    return (seq_t)pal;
}

/* Return 10^k as a 64‑bit unsigned integer (k <= 18). */
static uint64_t pow10_uint64(int k) {
    uint64_t v = 1;
    while (k--) v *= 10;
    return v;
}
