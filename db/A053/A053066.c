/*
 * A053066  Alternately append n-th prime to end or beginning of previous
 *          term.
 * https://oeis.org/A053066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, less
 * Formula:   a(1)=2; for n>1 let p = nth prime. If n is even: a(n) =
 *            a(n‑1)*10^digits(p) + p (append) If n is odd : a(n) =
 *            p*10^len(a(n‑1)) + a(n‑1) (prepend)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   digits_uint32, pow10_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digits_uint32.h"
#include "../utils/pow10_small.h"

/* Seq_t and idx_t are supplied by the build. */

/* --- Prime generation --------------------------------------------- */
static uint32_t *primes = NULL;          /* array of found primes */
static size_t primes_len   = 0;          /* number of primes stored   */
static size_t primes_cap   = 0;          /* allocated capacity        */
static uint32_t next_candidate = 2;      /* next number to test       */

static void ensure_primes_at_least(size_t n)
{
    if (primes_len >= n) return;

    while (primes_len < n) {
        uint32_t p = next_candidate;
        bool is_prime = true;
        uint64_t p2 = (uint64_t)p * (uint64_t)p;
        for (size_t i = 0; i < primes_len; ++i) {
            uint32_t q = primes[i];
            if ((uint64_t)q * (uint64_t)q > p) break;
            if (p % q == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_cap == 0) {
                primes_cap = 16;
                primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
            } else if (primes_len >= primes_cap) {
                primes_cap *= 2;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_len++] = p;
        }
        ++next_candidate;
    }
}

/* --- Sequence storage ---------------------------------------------- */
static seq_t *terms   = NULL;   /* computed terms */
static size_t *len_arr = NULL;  /* decimal lengths of terms */
static size_t terms_len = 0;    /* number of terms stored so far */
static size_t terms_cap = 0;    /* allocated capacity */

/* Store a term given its value and length.  Expand storage if needed. */
static void store_term(seq_t value, size_t len)
{
    if (terms_len >= terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        if (new_cap < terms_len + 1) new_cap = terms_len + 1;
        terms     = (seq_t *)realloc(terms,     new_cap * sizeof(seq_t));
        len_arr   = (size_t *)realloc(len_arr,   new_cap * sizeof(size_t));
        terms_cap = new_cap;
    }
    terms[terms_len]   = value;
    len_arr[terms_len] = len;
    ++terms_len;
}

/* --- The sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1ULL) return (seq_t)0;    /* out of range */

    /* Ensure entries up to n are computed. */
    while ((size_t)n > terms_len) {
        size_t idx = terms_len + 1;    /* new term index (1‑based) */

        /* Base case: a(1) = 2 */
        if (idx == 1) {
            store_term((seq_t)2, 1);
            continue;
        }

        /* Make sure we have the idx‑th prime. */
        ensure_primes_at_least(idx);
        uint32_t prime_val = primes[idx - 1];
        seq_t   prime      = (seq_t)prime_val;
        int     d          = digits_uint32(prime_val);
        seq_t   pow10_d    = pow10_small(d);

        /* Length of the previous term. */
        size_t prev_len = len_arr[terms_len - 1];
        seq_t  prev     = terms[terms_len - 1];

        /* For odd indices we need 10^prev_len.  Compute it on the fly once. */
        static seq_t pow10_prev = 0;   /* 10^(previous length) */
        if (pow10_prev == 0) pow10_prev = 10;   /* After a(1) it is 10^1 */

        seq_t new_term;
        if ((idx & 1ULL) == 0ULL) {           /* even: append */
            new_term = prev * pow10_d + prime;
        } else {                               /* odd: prepend */
            new_term = prime * pow10_prev + prev;
        }

        size_t new_len = prev_len + (size_t)d;

        /* Update pow10_prev for the next odd step. */
        pow10_prev *= pow10_d;

        store_term(new_term, new_len);
    }

    return terms[n - 1];
}
