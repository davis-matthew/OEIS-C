/*
 * A215850  Primes p such that 2*p + 1 divides Lucas(p).
 * https://oeis.org/A215850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 277, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A215850: primes p such that 2p+1 divides Lucas(p).
   Computed by generating primes p ≡ 29 (mod 30) for which 2p+1 is also
   prime, plus the initial special term 5.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *terms = NULL;       /* cached terms of the sequence   */
static size_t terms_cap = 0;      /* allocated capacity             */
static size_t terms_len = 0;      /* number of terms currently cached */
static idx_t next_k = 0;          /* next k for candidate p = 30·k+29 */

/* Ensure the cache can hold at least 'needed' terms. */
static inline void ensure_terms_cap(size_t needed)
{
    if (needed <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);                       /* out‑of‑memory – abort */
    terms = tmp;
    terms_cap = new_cap;
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for 0, return 0 as safe */

    /* Initialization: first term is 5. */
    if (terms_len == 0) {
        ensure_terms_cap(1);
        terms[0] = (seq_t)5;
        terms_len = 1;
        next_k = 0;                     /* start checking p = 29   */
    }

    /* Generate terms until we reach the requested index. */
    while (terms_len < n) {
        idx_t p = 30 * next_k + 29;
        next_k++;

        if (is_prime(p)) {
            idx_t two_p_plus1 = 2 * p + 1;
            if (is_prime(two_p_plus1)) {
                ensure_terms_cap(terms_len + 1);
                terms[terms_len] = (seq_t)p;
                terms_len++;
            }
        }
    }

    return terms[n - 1];
}
