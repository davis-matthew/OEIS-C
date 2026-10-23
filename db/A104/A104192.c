/*
 * A104192  a(n) = prime(n) - phi(n).
 * https://oeis.org/A104192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) - phi(n)
 * Range:     u8 n <= 59, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   compute_phi, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: prime(n) – the n‑th prime (2,3,5,…) and phi(n) – Euler's totient function. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"      /* bool is_prime(uint64_t n); */
#include "../utils/compute_phi.h"  /* uint64_t compute_phi(uint64_t n); */

/* --------------------------------------------------------------------- */
/*  Storage for known primes                                               */
static idx_t   prime_cnt   = 0;       /* number of primes stored */
static uint64_t *prime_list = NULL;  /* array of primes (1‑based via index‑1) */

/* --------------------------------------------------------------------- */
/*  Ensure that prime_list contains at least n primes.                    */
static void ensure_primes(idx_t n)
{
    while (prime_cnt < n) {
        uint64_t candidate;
        if (prime_cnt == 0) {
            candidate = 2;
        } else {
            /* start searching just after the last known prime */
            candidate = prime_list[prime_cnt - 1] + 1;
        }

        /* Simple trial division using the primes we already know.   */
        while (!is_prime(candidate)) {
            candidate++;
        }

        /* Grow the array by one entry. */
        uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                            (prime_cnt + 1) * sizeof(uint64_t));
        /* Allocation failure is ignored – the runtime will abort if it
           happens, which is acceptable for OEIS evaluators.          */
        prime_list = tmp;
        prime_list[prime_cnt++] = candidate;
    }
}

/* --------------------------------------------------------------------- */
/*  The required sequence function.                                       */
seq_t a(idx_t n)
{
    /* a(1) corresponds to the first prime (2) and φ(1) = 1.           */
    if (n < 1) return (seq_t)0;          /* undefined, but harmless */

    ensure_primes(n);

    uint64_t p  = prime_list[n - 1];     /* n-th prime */
    uint64_t ph = compute_phi(n);        /* φ(n)   */

    return (seq_t)(p - ph);
}
