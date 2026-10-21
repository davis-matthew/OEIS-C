/*
 * A211396  Smallest prime substring of n, or 0 if no such substring exists.
 * https://oeis.org/A211396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 400, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   idx_to_string, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A211396: Smallest prime substring of n, or 0 if none.
   Implementation uses brute‑force generation of all decimal
   substrings and tests each for primality. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/idx_to_string.h"

seq_t a(idx_t n) {
    char s[32];
    size_t len;
    idx_to_string(n, s, &len);

    seq_t best = 0;
    bool found = false;

    for (size_t i = 0; i < len; ++i) {
        seq_t val = 0;
        for (size_t j = i; j < len; ++j) {
            val = val * 10 + (seq_t)(s[j] - '0');
            if (is_prime_big(val)) {
                if (!found || val < best) {
                    best = val;
                    found = true;
                }
            }
        }
    }
    return found ? best : (seq_t)0;
}
