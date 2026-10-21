/*
 * A090194  Primes which when multiplied by their largest digit and 1 is added
 *          form another prime.
 * https://oeis.org/A090194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 41, u32 n <= 54, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, get_digits, is_prime_big, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A090194: primes p such that p * max_digit(p) + 1 is also prime.
   The function returns the n‑th such prime (a(1) = 2).  The sequence is
   generated on demand and memoised; calls may arrive in any order. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/get_digits.h"
#include "../utils/is_prime_big.h"
#include "../utils/mul_small.h"
#include "../utils/ensure_capacity.h"

/* Return the n-th term of the sequence.  n ≥ 1. */
seq_t a(idx_t n)
{
    /* Static cache of found terms. */
    static seq_t *seq      = NULL;
    static size_t seq_len  = 0;
    static size_t seq_cap  = 0;

    /* Static state used to continue the search between calls. */
    static seq_t next_candidate = 2;   /* next number to test for primality */

    while (seq_len < n) {
        /* Find the next prime number. */
        while (!is_prime_big(next_candidate))
            next_candidate += 1;          /* candidate may be a big integer */

        /* Now next_candidate is prime.  Compute its largest decimal digit. */
        size_t dlen;
        uint8_t digits[128];
        get_digits(next_candidate, digits, &dlen);   /* digits[0..dlen-1] */

        unsigned max_digit = 0;
        for (size_t i = 0; i < dlen; ++i)
            if (digits[i] > max_digit)
                max_digit = digits[i];

        /* Compute next_candidate * max_digit + 1 and test primality. */
        seq_t prod = mul_small(next_candidate, max_digit);
        seq_t test = prod + 1;
        if (is_prime_big(test)) {
            /* Cache the prime itself. */
            ensure_capacity(seq_len + 1, &seq_cap,
                            (void **)&seq, sizeof(seq_t));
            seq[seq_len++] = next_candidate;
        }

        /* Advance to the next integer to test. */
        next_candidate += 1;
    }

    return seq[n - 1];
}
