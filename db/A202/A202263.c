/*
 * A202263  Primes in which all substrings and reversal substrings are primes.
 * https://oeis.org/A202263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   get_digits, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A202263: primes whose every decimal substring
 * and every reverse of such substrings is also prime. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_small.h"
#include "../utils/get_digits.h"

/* Return true iff p is a prime and all its decimal substrings and
 * the reversals of those substrings are also primes. */
static bool substrings_all_prime(seq_t p)
{
    /* Extract decimal digits of p into buf[0..len-1], most
     * significant digit first. */
    uint8_t buf[32];
    size_t len;
    get_digits(p, buf, &len);

    for (size_t i = 0; i < len; ++i) {
        seq_t val = 0;
        for (size_t j = i; j < len; ++j) {
            val = val * 10 + buf[j];

            /* Build the reverse of the substring buf[i..j] */
            seq_t rev = 0;
            for (int k = (int)j; k >= (int)i; --k) {
                rev = rev * 10 + buf[k];
            }

            if (!is_prime_small(val) || !is_prime_small(rev))
                return false;
        }
    }
    return true;
}

/* Return the n-th element of A202263 (1‑based).  For n>7
 * the sequence is finite, so we return 0. */
seq_t a(idx_t n)
{
    static seq_t cache[7];
    static idx_t filled = 0;

    if (filled == 0) {
        seq_t cand = 2;
        while (filled < 7) {
            if (is_prime_small(cand) && substrings_all_prime(cand))
                cache[filled++] = cand;
            ++cand;
        }
    }

    if (n == 0 || n > 7)
        return 0;
    return cache[n - 1];
}
