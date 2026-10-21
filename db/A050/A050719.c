/*
 * A050719  Inserting a digit '9' between adjacent digits of n makes a prime.
 * https://oeis.org/A050719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = element n in OEIS A050719 found by brute‑force search;
 *            the sequence consists of those integers n for which inserting a
 *            digit '9' between each pair of adjacent decimal digits yields a
 *            prime
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 90, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Test whether inserting a '9' between every pair of adjacent digits
   of 'm' produces a prime. */
static bool has_prime_interleave(seq_t m)
{
    if (m < 10) return false;   /* single–digit m has no adjacent pair */

    /* Extract decimal digits of m (least significant first). */
    unsigned char digits[64];
    size_t dlen = 0;
    seq_t tmp = m;
    while (tmp > 0) {
        digits[dlen++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }

    /* Build the interleaved number: most‑significant digit first. */
    seq_t inter = 0;
    for (size_t i = dlen; i > 0; ) {
        --i;
        inter = inter * 10 + digits[i];
        if (i > 0) {
            inter = inter * 10 + (seq_t)9;
        }
    }

    return is_prime_big(inter);
}

/* Return the n‑th element of sequence A050719 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but not used */

    seq_t count = 0;   /* how many suitable numbers we have found */
    seq_t cand   = 1;   /* current candidate to test */

    while (count < n) {
        if (has_prime_interleave(cand)) {
            ++count;
            if (count == n) return cand;
        }
        ++cand;
    }
    /* Unreachable, but required for compiler safety. */
    return 0;
}
