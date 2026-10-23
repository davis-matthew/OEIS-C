/*
 * A140563  Primes p such that p + 38 is not a prime.
 * https://oeis.org/A140563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A140563: primes p such that p + 38 is not a prime. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Check primality of an idx_t value.  Works for 2, 3 and all odd numbers. */
static bool is_prime_idx(idx_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;   /* even number > 2 */

    /* test divisibility by odd candidates up to sqrt(n) */
    for (idx_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Static state to allow non‑sequential calls. */
    static seq_t *output = NULL;   /* stored primes with the property */
    static idx_t out_cap = 0;      /* allocated capacity */
    static idx_t out_count = 0;    /* number of stored primes */
    static idx_t next = 2;         /* next integer to examine */

    /* Initialise storage on first call. */
    if (output == NULL) {
        out_cap = 128;
        output = (seq_t *)malloc(out_cap * sizeof(seq_t));
        out_count = 0;
        next = 2;
    }

    /* Generate primes until we have at least n of them. */
    while (out_count < n) {
        /* Test whether the candidate is prime. */
        if (is_prime_idx(next)) {
            /* Check whether p + 38 is also prime. */
            if (!is_prime_idx(next + 38)) {
                /* Ensure there is room. */
                if (out_count >= out_cap) {
                    out_cap *= 2;
                    output = (seq_t *)realloc(output, out_cap * sizeof(seq_t));
                }
                output[out_count++] = (seq_t)next;
            }
        }

        /* Advance to the next candidate. */
        if (next == 2)
            next = 3;
        else
            next += 2;   /* skip even numbers */
    }

    return output[n - 1];
}
