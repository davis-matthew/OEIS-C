/*
 * A092844  a(n) = Sum_{k=1..n} prime(k)*10^(k-1).
 * https://oeis.org/A092844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..n} prime(k) * 10^(k-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Compute the n-th term of A092844.  The function is re‑entrant, caches
   primes between calls, and works for any idx_t ≥ 1. */
seq_t a(idx_t n)
{
    /* --- Prime generation ----------------------------------------- */
    static uint32_t *primes      = NULL;   /* primes[i] = i-th prime (1‑based) */
    static size_t   primes_len   = 0;
    static size_t   primes_cap   = 0;
    static uint32_t candidate    = 2;      /* next integer to test for primality */

    /* Ensure we have at least n primes. */
    while (primes_len < n) {
        /* Add 'candidate' to the list if it is prime. */
        bool is_prime = true;
        /* Trial division using known primes. */
        for (size_t i = 0; i < primes_len; ++i) {
            uint32_t p = primes[i];
            if ((uint32_t)p * (uint32_t)p > candidate) {
                break;
            }
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            /* Grow array if needed. */
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                uint32_t *tmp = (uint32_t*)realloc(primes, new_cap * sizeof(uint32_t));
                if (!tmp) return (seq_t)0; /* allocation failed; return 0 as safe fallback */
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = candidate;
        }
        ++candidate;
    }

    /* --- Compute a(n) ---------------------------------------------- */
    seq_t  result  = (seq_t)0;
    seq_t  pow10   = (seq_t)1;   /* 10^(k-1) for the current k (starting at k=1) */

    for (size_t k = 0; k < (size_t)n; ++k) {
        seq_t pval = (seq_t)primes[k];
        result = result + (pval * pow10);
        pow10  = pow10 * (seq_t)10;
    }
    return result;
}
