/*
 * A084368  Numbers k such that prime(k) does not contain the digit 1.
 * https://oeis.org/A084368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_no_digit_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A084368 – Numbers k such that prime(k) does not contain the digit 1.
 *  The function returns the n‑th qualifying k (1‑based indexing).
 *
 *  Implementation strategy:
 *      • Generate primes incrementally by trial division.
 *      • Maintain a list of indices k whose prime(k) has no digit 1.
 *      • On each call, extend the list until it contains the requested
 *        n‑th term and return it.
 *      • All state is stored in static variables so that multiple calls
 *        work correctly in any order.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_no_digit_one.h"

/* -----------  Static state  -------------------------------------- */
static uint32_t *prime_list = NULL;          /* primes found so far       */
static size_t prime_cap    = 0;              /* capacity of prime_list    */
static size_t prime_used   = 0;              /* number of primes stored  */
static uint32_t prime_max_checked = 1;      /* largest integer tested   */

static seq_t *qual_terms   = NULL;          /* list of qualifying k's   */
static size_t terms_cap    = 0;              /* capacity of qual_terms   */
static size_t terms_used   = 0;              /* number of terms stored   */

static uint32_t next_k      = 1;             /* next k to test           */

/* -----------  Helper functions  ----------------------------------- */
static void ensure_primes(size_t need)
{
    while (prime_used < need) {
        ++prime_max_checked;
        uint32_t p = prime_max_checked;
        bool is_prime = true;
        for (size_t i = 0; i < prime_used; ++i) {
            uint32_t d = prime_list[i];
            uint64_t d2 = (uint64_t)d * d;
            if (d2 > p) break;
            if (p % d == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_used == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 32;
                uint32_t *tmp = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
                if (!tmp) abort();
                prime_list = tmp;
            }
            prime_list[prime_used++] = p;
        }
    }
}

/* -----------  Sequence function  --------------------------------- */
seq_t a(idx_t n)
{
    /* extend the list until we have at least n terms */
    while (terms_used < (size_t)n) {
        /* make sure the k-th prime is known */
        ensure_primes((size_t)next_k);

        uint32_t p_k = prime_list[next_k - 1];   /* primes are zero‑indexed */

        if (has_no_digit_one(p_k)) {
            if (terms_used == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(qual_terms, terms_cap * sizeof(seq_t));
                if (!tmp) abort();
                qual_terms = tmp;
            }
            qual_terms[terms_used++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return qual_terms[n - 1];
}
