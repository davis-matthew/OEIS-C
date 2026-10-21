/*
 * A079148  Primes p such that p-1 has at most 2 prime factors, counted with
 *          multiplicity; i.e., primes p such that bigomega(p-1) =
 *          A001222(p-1) <= 2.
 * https://oeis.org/A079148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th prime p with bigomega(p‑1) ≤ 2 (where bigomega
 *            counts prime factors of p‑1 with multiplicity). Indexing starts
 *            at 1 (a(1) = 2). The function generates primes on demand and
 *            memoises all found terms
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   append_prime, bigomega_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/append_prime.h"
#include "../utils/bigomega_uint64.h"

/* Helper to append a value to a dynamic array */
static void append_term(seq_t **arr, size_t *cnt, size_t *cap, seq_t val)
{
    if (*cnt >= *cap) {
        *cap = (*cap == 0) ? 64 : (*cap << 1);
        *arr = (seq_t *)realloc(*arr, *cap * sizeof(seq_t));
    }
    (*arr)[*cnt] = val;
    (*cnt)++;
}

/* Globally cached state */
static uint64_t *prime_list    = NULL; /* stored primes as uint64_t */
static size_t   prime_cnt      = 0;
static size_t   prime_cap      = 0;
static seq_t   *terms          = NULL; /* stored sequence terms as seq_t */
static size_t   terms_cnt      = 0;
static size_t   terms_cap      = 0;
static uint64_t next_candidate = 2;     /* next number to test for primality */

/* Ensures we have computed at least n terms of the sequence. */
static void ensure_terms_up_to(idx_t n)
{
    while (terms_cnt < (size_t)n) {
        /* Test primeness of next_candidate */
        bool is_prime = true;
        if (next_candidate < 2) is_prime = false;
        else if (next_candidate == 2) is_prime = true;
        else {
            for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= next_candidate; ++i) {
                if (next_candidate % prime_list[i] == 0) {
                    is_prime = false;
                    break;
                }
            }
        }

        if (is_prime) {
            uint64_t p = next_candidate;
            append_prime(&prime_list, &prime_cnt, &prime_cap, p);

            /* Now test bigomega(p-1) */
            uint8_t omega = bigomega_uint64(p - 1, prime_list, prime_cnt);
            if (omega <= 2) {
                append_term(&terms, &terms_cnt, &terms_cap, (seq_t)p);
            }
        }

        /* Move to the next candidate for primality testing */
        if (next_candidate == 2) next_candidate = 3;
        else                       next_candidate += 2;  /* skip even numbers */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n < 1 */
    ensure_terms_up_to(n);
    return terms[n - 1];
}
