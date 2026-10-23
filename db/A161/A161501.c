/*
 * A161501  a(n) is the smallest positive integer that is a palindrome when
 *          represented in binary, and is made by appending some number (zero
 *          or more) of binary digits on the right of n's binary
 *          representation.
 * https://oeis.org/A161501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the smallest positive integer obtained by appending a
 *            finite number of binary digits to the right of the binary
 *            representation of n so that the resulting number is a binary
 *            palindrome
 * Range:     u8 n <= 29, u16 n <= 509, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Attempt to build a palindrome of length len + k bits.
   The bits of n (length len) are placed starting at position k.
   Return 0 if no palindrome can be formed for this k. */
static seq_t build_palindrome(seq_t n_val, size_t len, size_t k)
{
    size_t total = len + k;
    if (total > 128) return 0;           /* safety guard */

    int8_t bits[128];
    for (size_t i = 0; i < total; ++i) bits[i] = -1;   /* unknown */

    /* Place bits of n into the correct positions. */
    for (size_t i = 0; i < len; ++i)
        bits[i + k] = (int8_t)((n_val >> i) & 1);

    /* Enforce palindrome constraints. */
    for (size_t i = 0; i < total / 2; ++i) {
        size_t j = total - 1 - i;
        int8_t a = bits[i];
        int8_t b = bits[j];
        if (a != -1 && b != -1) {
            if (a != b) return 0;           /* mismatch */
        } else if (a == -1 && b != -1) {
            bits[i] = b;
        } else if (a != -1 && b == -1) {
            bits[j] = a;
        } else {
            /* both unknown: choose 0 for minimal value */
            bits[i] = bits[j] = 0;
        }
    }
    /* Handle middle bit for odd length. */
    if (total % 2 == 1) {
        size_t mid = total / 2;
        if (bits[mid] == -1) bits[mid] = 0;
    }

    /* Assemble the resulting number. */
    seq_t val = 0;
    for (size_t i = 0; i < total; ++i) {
        val <<= 1;
        val |= (seq_t)bits[i];
    }
    return val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety guard for the offset */

    seq_t n_val = (seq_t)n;
    /* Determine the bit length of n (at least 1). */
    size_t len = 0;
    seq_t tmp = n_val;
    while (tmp) {
        tmp >>= 1;
        ++len;
    }

    /* Try increasing numbers of appended bits until a palindrome is found. */
    for (size_t k = 0; k <= len; ++k) {
        seq_t cand = build_palindrome(n_val, len, k);
        if (cand != 0) return cand;
    }
    /* A palindrome must always exist; return n as fallback. */
    return n_val;
}
