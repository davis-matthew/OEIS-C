/*
 * A147873  A sequence based on the mechanics of A147781: b(n) = Apply[Plus,
 *          IntegerDigits( 17982*Sum_{m=0..7} Prime(n+m) )]; a(n) = 1-(b(n)
 *          mod 2).
 * https://oeis.org/A147873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less, uned
 * Formula:   a(n) = 1 - (b(n) mod 2) * where b(n) = digit sum of 17982 *
 *            Σ_{m=0..7} Prime(n+m)
 * Range:     u8 n <= 3, u16 none, u32 n <= 100, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   get_digits, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/get_digits.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1: a(1) is the first term. */
    if (n == 0) return (seq_t)0;      /* defensive: undefined for n<1 */

    /* Sum of 8 consecutive primes starting at the nth prime. */
    seq_t sum = 0;
    for (idx_t i = 0; i < 8; ++i)
        sum += (seq_t)get_prime(n + i);

    /* Product with the constant 17982. */
    seq_t prod = sum * (seq_t)17982;

    /* Extract decimal digits of prod and compute parity of their sum. */
    uint8_t digits[64];
    size_t len = 0;
    get_digits(prod, digits, &len);

    unsigned parity = 0;               /* parity of digit sum (mod 2) */
    for (size_t i = 0; i < len; ++i)
        parity ^= (digits[i] & 1u);

    /* a(n) = 1 if digit sum even, else 0. */
    return (parity == 0) ? (seq_t)1 : (seq_t)0;
}
