/*
 * A229107  Palindromic prime time display in hours, minutes, seconds on a
 *          six-digit 24-hour digital clock.
 * https://oeis.org/A229107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 45, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   is_palindrome, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A229107 by brute‑force enumeration of all times 00:00:00‑23:59:59,
   keeping only those whose numeric representation (leading zeros omitted)
   is a palindrome in base 10 and a prime.  The list contains 69 terms. */

#include <stdbool.h>
#include "../utils/is_palindrome.h"
#include "../utils/is_prime_big.h"

static seq_t cache[70];
static idx_t cached_len = 0;
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = true;
        cached_len = 0;

        for (idx_t h = 0; h < 24; ++h) {
            for (idx_t m = 0; m < 60; ++m) {
                for (idx_t s = 0; s < 60; ++s) {
                    /* numeric value after removing leading zeros: hhmmss */
                    idx_t val = h * 10000 + m * 100 + s;
                    seq_t val_seq = (seq_t)val;
                    if (is_palindrome(val_seq, 10) && is_prime_big(val_seq)) {
                        if (cached_len < 70) {
                            cache[cached_len++] = val_seq;
                        }
                    }
                }
            }
        }
    }

    if (n >= 1 && n <= cached_len) {
        return cache[n - 1];
    }
    return (seq_t)0; /* out‑of‑range request */
}
