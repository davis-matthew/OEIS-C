/*
 * A102497  Primes of the concatenated form 1nn1 where n is an integer >=0.
 * https://oeis.org/A102497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   isprime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses incremental search over n to find the nth prime of the form 1nn1. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/isprime_big.h"

/* Compute 10^exp as a seq_t. */
static seq_t pow10_seq(uint64_t exp)
{
    seq_t res = 1;
    for (uint64_t i = 0; i < exp; ++i)
        res *= 10;  /* seq_t * int is allowed by ordinary arithmetic conversions */
    return res;
}

/* Return the number of decimal digits of an idx_t value (at least 1). */
static idx_t num_digits(idx_t x)
{
    idx_t d = 0;
    do {
        ++d;
        x /= 10;
    } while (x > 0);
    return d;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for 0‑based index, but offset starts at 1 */

    idx_t count = 0;
    idx_t m = 0;           /* current middle value n in 1nn1 */
    while (true) {
        idx_t k = num_digits(m);          /* number of digits in m */
        seq_t pow_k1   = pow10_seq(k + 1); /* 10^(k+1)   */
        seq_t pow_2k1  = pow10_seq(2 * k + 1); /* 10^(2k+1) */

        seq_t val = pow_2k1
                  + (seq_t)m * pow_k1
                  + (seq_t)m * 10
                  + 1;               /* 1nn1 */

        if (isprime_big(val)) {
            ++count;
            if (count == n)
                return val;
        }
        ++m;
    }
}
