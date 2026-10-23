/*
 * A157869  Palindromic primes having both a prime middle digit and a prime
 *          digital root.
 * https://oeis.org/A157869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 19, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   digit_sum, is_prime_big, pow10, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: palindromic primes having both a prime middle digit and a prime digital root */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/digit_sum.h"
#include "../utils/reverse_digits.h"
#include "../utils/pow10.h"

/* Return true if the digital root of v is a prime digit (2,3,5,7). */
static bool digit_root_prime(seq_t v)
{
    seq_t dr = v;
    while (dr >= (seq_t)10) {
        dr = digit_sum(dr);
    }
    return dr == (seq_t)2 || dr == (seq_t)3 || dr == (seq_t)5 || dr == (seq_t)7;
}

seq_t a(idx_t n)
{
    /* Prime digits that can occur in the middle of the palindrome. */
    static const int mid_digits[4] = {2, 3, 5, 7};
    idx_t count = 0;

    /* Palindromic primes of length 1 (single-digit primes). */
    for (size_t i = 0; i < 4; ++i) {
        seq_t cand = (seq_t)mid_digits[i];
        if (is_prime_big(cand) && digit_root_prime(cand)) {
            ++count;
            if (count == n)
                return cand;
        }
    }

    /* Generate odd‑length palindromes of length 3,5,7,… in increasing order. */
    for (idx_t len = 3; ; len += 2) {
        idx_t half = (len - 1) / 2;          /* number of digits on one side          */
        seq_t start = pow10(half - 1);        /* 10^(half‑1) – leftmost non‑zero digit */
        seq_t end   = pow10(half);            /* 10^half                          */

        seq_t pow10_half     = pow10(half);
        seq_t pow10_half_plus= pow10(half + 1);

        for (seq_t left = start; left < end; ++left) {
            seq_t rev_left = reverse_digits(left);
            for (size_t i = 0; i < 4; ++i) {
                seq_t cand = left * pow10_half_plus
                           + (seq_t)mid_digits[i] * pow10_half
                           + rev_left;
                if (is_prime_big(cand) && digit_root_prime(cand)) {
                    ++count;
                    if (count == n)
                        return cand;
                }
            }
        }
    }
    /* The function will always return inside the loops. */
    return (seq_t)0;
}
