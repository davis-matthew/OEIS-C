/*
 * A034808  Concatenation of 'prevprime(k) and k' is a prime.
 * https://oeis.org/A034808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 8, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   digits, is_prime_big, is_prime_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A034808: the n-th integer k (k ≥ 1) such that
   concatenation of the previous prime of k and k is prime.
   Example: for k = 333, prevprime = 331 → “331333” is prime,
   so 333 appears in the sequence. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "../utils/is_prime_small.h"   /* bool is_prime_small(seq_t n) */
#include "../utils/is_prime_big.h"     /* bool is_prime_big(seq_t n) */
#include "../utils/digits.h"           /* int digits(idx_t x) */
#include "../utils/pow10.h"            /* seq_t pow10(idx_t e) */

/* Grow a dynamic array of seq_t.  The array is reallocated by C++-style
   cast for the allow of non‑POD seq_t types in the arbitrary‑precision
   build. */
static void grow_seq_t(seq_t **p, size_t *len, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 8;
        while (new_cap < need) new_cap *= 2;
        /* explicit cast makes it valid C++ for non‑trivial seq_t types */
        seq_t *tmp = (seq_t *)realloc(*p, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *p = tmp;
        *cap = new_cap;
    }
}

/* Grow a dynamic array of idx_t.  Uses a C‑style cast for C++ builds. */
static void grow_idx_t(idx_t **p, size_t *len, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 8;
        while (new_cap < need) new_cap *= 2;
        idx_t *tmp = (idx_t *)realloc(*p, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        *p = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* 0 is unused; the sequence is 1‑indexed (offset = 1) */
    if (n == 0) return (seq_t)0;

    /* Static state to allow arbitrary call order */
    static idx_t last_k_done = 1;   /* last integer k processed          */
    static seq_t *terms = NULL;    /* the list of sequence values       */
    static size_t terms_len = 0, terms_cap = 0;
    static idx_t *primes = NULL;   /* primes ≤ last_k_done              */
    static size_t primes_len = 0, primes_cap = 0;

    /* Generate terms until we have at least n items */
    while (terms_len < n) {
        ++last_k_done;
        bool k_is_prime = is_prime_small((seq_t)last_k_done);

        /* maintain the prime list */
        if (k_is_prime) {
            grow_idx_t(&primes, &primes_len, &primes_cap, primes_len + 1);
            primes[primes_len++] = last_k_done;
        }

        /* find previous prime of last_k_done (variant 2) */
        idx_t prevp = 0;
        if (primes_len == 0) {
            prevp = 0;            /* no previous prime yet           */
        } else if (k_is_prime) {
            if (primes_len >= 2)
                prevp = primes[primes_len - 2];
        } else {
            prevp = primes[primes_len - 1];
        }

        if (prevp >= 2) {
            int dk = digits(last_k_done);
            seq_t pow10d = pow10((idx_t)dk);
            seq_t candidate = (seq_t)prevp * pow10d + (seq_t)last_k_done;

            if (is_prime_big(candidate)) {
                grow_seq_t(&terms, &terms_len, &terms_cap, terms_len + 1);
                terms[terms_len++] = (seq_t)last_k_done;
            }
        }
    }

    return terms[n - 1];
}
