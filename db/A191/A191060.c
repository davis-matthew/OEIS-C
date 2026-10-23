/*
 * A191060  Primes that are not squares mod 11.
 * https://oeis.org/A191060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes that are not squares mod 11
 * The n‑th term is the n‑th prime p such that p mod 11 ∈ {2,6,7,8,10}.
 * Computed by incremental prime testing with memoisation. */

#include <stdlib.h>
#include <stdbool.h>

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)           /* undefined, but keep it safe */
        return (seq_t)0;

    /* --- static state used for memoisation --- */
    static idx_t candidate = 2;                 /* next number to test */
    static idx_t *prime_list = NULL;            /* list of all primes found */
    static size_t prime_cap   = 0;
    static size_t prime_len   = 0;

    static seq_t *terms      = NULL;            /* terms that satisfy the condition */
    static size_t terms_cap  = 0;
    static size_t terms_len  = 0;               /* number of terms currently stored */

    /* --- ensure we have the first n terms --- */
    while (terms_len < (size_t)n)
    {
        /* Check primality of 'candidate' by trial division */
        bool is_prime = true;
        for (size_t i = 0; i < prime_len; ++i)
        {
            idx_t p = prime_list[i];
            if (p > candidate / p) break;          /* p^2 > candidate, stop */
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
        {
            /* Add candidate to prime list */
            if (prime_len == prime_cap)
            {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
                if (!tmp) abort();                 /* allocation failure */
                prime_list = tmp;
                prime_cap  = new_cap;
            }
            prime_list[prime_len++] = candidate;

            /* Check if candidate satisfies the residue condition */
            idx_t mod = candidate % 11;
            if (mod != 0 && (mod == 2 || mod == 6 || mod == 7 || mod == 8 || mod == 10))
            {
                if (terms_len == terms_cap)
                {
                    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    terms      = tmp;
                    terms_cap  = new_cap;
                }
                terms[terms_len++] = (seq_t)candidate;
            }
        }

        ++candidate;   /* move to the next integer */
    }

    return terms[n - 1];
}
