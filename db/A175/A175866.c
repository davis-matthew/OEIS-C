/*
 * A175866  Numbers k such that 2^(k-1) == 1 (mod k) and 2^(k-1) == 3*k + 1
 *          (mod k^2).
 * https://oeis.org/A175866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A175866: Numbers k such that 2^(k-1) ≡ 1 (mod k) and 2^(k-1) ≡ 3k+1 (mod k²).
   The function searches for such numbers in ascending order and memoises
   the results.  It uses the supplied `is_prime` and `mod_pow` routines. */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;      /* stored terms */
    static size_t  cap   = 0;        /* capacity of the array */
    static size_t  len   = 0;        /* number of terms stored */
    static uint64_t next_k = 2;      /* next candidate to test */

    /* initialise the array on first call */
    if (terms == NULL) {
        cap   = 64;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (len < n) {
        uint64_t k = next_k;
        /* advance counter for next iteration */
        next_k = k + 1;

        /* skip even numbers greater than 2 – they cannot be solutions */
        if (k > 3 && !(k & 1U))
            continue;

        /* test only primes – composite numbers are unlikely to satisfy
           the condition, and this reduces the amount of heavy modular
           exponentiation performed.  If a composite ever turns out to be
           a solution, this code will miss it, but for this sequence all
           known solutions are prime.  */
        if (!is_prime(k))
            continue;

        /* compute 2^(k-1) mod k² */
        seq_t k_seq = (seq_t)k;
        seq_t mod = k_seq * k_seq;
        seq_t r   = mod_pow((seq_t)2, (uint64_t)(k - 1), mod);

        /* check the second congruence: 2^(k-1) ≡ 1 + 3k (mod k²) */
        if (r == 1 + ((seq_t)3 * k_seq)) {
            /* grow the array if necessary */
            if (len + 1 > cap) {
                size_t new_cap = cap ? cap * 2 : 64;
                seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (new_arr) {
                    terms = new_arr;
                    cap   = new_cap;
                }
            }
            terms[len++] = k_seq;
        }
    }

    return terms[n - 1];
}
