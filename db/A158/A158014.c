/*
 * A158014  Primes p such that (p-1)/8 is also prime.
 * https://oeis.org/A158014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*q + 1 where q is prime and 8*q+1 is also prime (primes
 *            p such that (p-1)/8 is prime)
 * Range:     u8 n <= 5, u16 n <= 116, u32 n <= 117, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Resize a seq_t array to hold at least 'need' elements. */
static void
ensure_cap_seq_t(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t newcap = (*cap == 0) ? 4 : (*cap << 1);
    while (newcap < need) newcap <<= 1;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

/* Sequence entry a(n). 1‑based indexing. */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* ---------- Memoisation ---------- */
    static seq_t  *terms  = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    static idx_t next_q = 2;   /* next integer to test for primality */

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        /* Find the next prime q. */
        while (!is_prime_big((seq_t)next_q)) ++next_q;
        idx_t q = next_q;
        ++next_q;  /* advance for next search */

        /* Compute p = 8*q + 1 and test primality. */
        seq_t p = ((seq_t)8 * (seq_t)q) + (seq_t)1;
        if (is_prime_big(p)) {
            ensure_cap_seq_t(&terms, &terms_cap, terms_len + 1);
            terms[terms_len++] = p;
        }
    }

    return terms[n - 1];
}
