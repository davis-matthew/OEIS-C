/*
 * A088155  Value of n-th digit in hexadecimal representation of n^n.
 * https://oeis.org/A088155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n^n / 16^n) mod 16 * Computes the n‑th hexadecimal digit
 *            of n^n, * using only integer arithmetic. * The implementation
 *            performs modular exponentiation * modulo 16^(n+1) with a custom
 *            big‑integer routine * that works for any n up to at least 10 000
 *            * (≈2 000 seconds per call limit is respe...
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   mul_mod_words, pow_mod_words
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mul_mod_words.h"
#include "../utils/pow_mod_words.h"

/* Main sequence function, as required. */
seq_t a(idx_t n)
{
    /* OEIS convention: a(0) = 1. */
    if (n == 0) return (seq_t)1;

    /* We need n^n modulo 16^(n+1).  Since 16 = 2^4,
       16^(n+1) = 2^(4*(n+1)).  The bit width is 4*(n+1). */
    size_t bits   = 4 * (n + 1);
    size_t words  = (bits + 63) / 64;          /* number of 64‑bit words */
    size_t words2 = words;                     /* keep a separate copy for safety */

    /* Allocate arrays for base, result, and temporary buffer. */
    uint64_t *base    = (uint64_t *)calloc(words, sizeof(uint64_t));
    uint64_t *result  = (uint64_t *)calloc(words, sizeof(uint64_t));
    if (!base || !result) abort();

    /* Base = n (in little‑endian). */
    base[0] = (uint64_t)n;

    /* Compute result = n^n mod 2^(64*words). */
    pow_mod_words(base, (uint64_t)n, result, words);

    /* The desired digit is the digit at position n in base‑16, i.e.
       the bits from 4*n to 4*n+3 of the full integer.  In the
       little‑endian 64‑bit representation this is:
         word_index = (4*n) / 64   = n/16
         shift_bits = (4*n) % 64   = ( (n & 15) * 4 )
       We need to take the little‑endian words result[word_index]
       and result[word_index+1] (the latter may be beyond the
       allocated array; it will be 0). */
    size_t word_index = (size_t)n >> 4;           /* n / 16 */
    size_t shift_bits = ((size_t)n & 15) * 4;     /* (n % 16) * 4 */

    uint64_t low  = result[word_index];
    uint64_t high = (word_index + 1 < words) ? result[word_index + 1] : 0;
    uint64_t digit;
    if (shift_bits == 0) {
        digit = low & 0xFULL;
    } else {
        digit = (low >> shift_bits) | (high << (64 - shift_bits));
        digit &= 0xFULL;
    }

    free(base);
    free(result);
    return (seq_t)digit;
}
