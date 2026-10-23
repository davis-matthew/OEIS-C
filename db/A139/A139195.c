/*
 * A139195  (prime(n)!-7)/7.
 * https://oeis.org/A139195
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)!-7)/7 (for n ≥ 4)
 * Range:     u8 none, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   factorial, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* For n < 4 the definition is not an integer; return 0. */
    if (n < 4U) {
        return (seq_t)0;
    }

    /* Obtain the n‑th prime (1‑based indexing). */
    uint32_t p = get_prime(n);

    /* Compute p! (the factorial of the prime). */
    seq_t fact = factorial((idx_t)p);

    /* Compute (p! – 7) / 7.  Exact division is guaranteed for n ≥ 4. */
    seq_t result = fact - (seq_t)7;
    result = result / (seq_t)7;

    return result;
}
