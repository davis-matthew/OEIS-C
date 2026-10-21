/*
 * A062403  Primes of the form 2*k*prime(k) + 1.
 * https://oeis.org/A062403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, get_prime, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A062403:
   a(n) = the n‑th prime of the form 2*k*prime(k)+1,  k ≥ 1. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/get_prime.h"          /* uint32_t get_prime(idx_t k); */
#include "../utils/is_prime.h"           /* bool is_prime(idx_t x);      */
#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t n);  */
#include "../utils/ensure_capacity.h"   /* void ensure_capacity(...);  */

/* Helper: test primality of a seq_t candidate. */
static inline bool cand_is_prime(seq_t cand)
{
    /* Use 64‑bit routine if the type fits; otherwise use the big routine. */
    if (sizeof(seq_t) <= sizeof(idx_t)) {
        return is_prime((idx_t)cand);
    } else {
        return is_prime_big(cand);
    }
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    /* Static storage for memoised results. */
    static seq_t *results = NULL;   /* array of found primes */
    static size_t res_len = 0;      /* number of terms stored */
    static size_t res_cap = 0;      /* capacity of results array */

    /* Static state tracking the next k value to test. */
    static idx_t next_k = 1;        /* next k to evaluate */

    /* Expand the results array until we have at least n terms. */
    while (res_len < n) {
        /* Compute candidate for current k. */
        uint32_t pk = get_prime(next_k);                       /* prime(k)   */
        seq_t cand = (seq_t)2 * (seq_t)next_k * (seq_t)pk + 1;  /* 2k·pk + 1 */

        if (cand_is_prime(cand)) {
            /* Store the prime if it satisfies the form. */
            ensure_capacity(res_len + 1, &res_cap,
                            (void **)&results, sizeof(seq_t));
            results[res_len++] = cand;
        }
        /* Move to the next k. */
        ++next_k;
    }

    return results[n - 1];
}
