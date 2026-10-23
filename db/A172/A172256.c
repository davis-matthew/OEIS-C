/*
 * A172256  Primes p such that 2*p+-3 are both nonprimes.
 * https://oeis.org/A172256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A172256: primes p such that 2p-3 and 2p+3 are both non‑prime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

/* Computes the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* undefined for n=0 */

    /* ----- static cache of the sequence ----- */
    static seq_t *seq      = NULL;             /* array of found primes   */
    static size_t seq_len  = 0;                /* number of terms saved   */
    static size_t seq_cap  = 0;                /* current capacity        */

    /* ----- static state for prime generation ----- */
    static uint64_t candidate = 2;             /* next integer to test    */
    static bool initialized = false;

    /* Initialise cache on first call */
    if (!initialized) {
        seq_cap = 16;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        initialized = true;
    }

    /* ---- generate primes until we have at least n terms ----- */
    while (seq_len < (size_t)n) {
        bool isPrime = false;

        /* Quick test for 2, then skip even numbers */
        if (candidate == 2) {
            isPrime = true;
        } else if (candidate % 2 != 0) {
            isPrime = is_prime_small((idx_t)candidate);
        }

        if (isPrime) {
            uint64_t p   = candidate;
            uint64_t a1  = 2 * p - 3;
            uint64_t a2  = 2 * p + 3;

            /* both 2p-3 and 2p+3 must be non‑prime */
            if (!is_prime_small((idx_t)a1) && !is_prime_small((idx_t)a2)) {
                if (seq_len >= seq_cap) {
                    seq_cap *= 2;
                    seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
                }
                seq[seq_len++] = (seq_t)p;
            }
        }

        ++candidate;
    }

    return seq[n - 1];   /* 1‑based index -> 0‑based array position */
}
