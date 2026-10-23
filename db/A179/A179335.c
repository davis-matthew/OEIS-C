/*
 * A179335  a(n) is the smallest prime which appears as a substring of the
 *          decimal representation of prime(n).
 * https://oeis.org/A179335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest prime that appears as a contiguous decimal
 *            substring of the n‑th prime (OEIS A179335)
 * Range:     u8 n <= 54, u16 n <= 6897, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_to_str.h"

seq_t a(idx_t n)
{
    /* Verify the offset: we expect n >= 1. */
    if (n < 1) return 0;

    /* get_prime is 1‑based: get_prime(1) = 2, get_prime(2) = 3, … */
    seq_t prime = (seq_t)get_prime(n);

    /* Convert the prime to its decimal string representation. */
    char buf[64];
    size_t len;
    seq_to_str(prime, buf, &len);
    if (len == 0) return 0;          /* should never happen for a prime */
    buf[len] = '\0';

    seq_t best = 0;
    bool found = false;

    /* Enumerate all contiguous decimal substrings of the prime. */
    for (size_t i = 0; i < len; ++i) {
        seq_t val = 0;
        for (size_t j = i; j < len; ++j) {
            val = val * 10 + (seq_t)(buf[j] - '0');
            if (is_prime_big(val)) {
                if (!found || val < best) {
                    best = val;
                    found = true;
                }
            }
        }
    }
    return best;
}
