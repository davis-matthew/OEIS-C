/*
 * A105327  Numbers n such that pi(n)=pi(d_1!)+pi(d_2!)+...+pi(d_k!) where d_1
 *          d_2 ...d_k is the decimal expansion of n.
 * https://oeis.org/A105327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A105327: numbers n such that pi(n)=∑ pi(d!) over the decimal digits d of n.
   The function enumerates all such n up to a fixed limit (1 000 000) and returns
   the nth element (1‑based indexing).  Since the sequence is proven to stop
   after a(19) (889 619), all further indices return 0. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static bool initialized = false;
static seq_t *terms    = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* Append an element to the dynamic array of terms. */
static void push_term(seq_t val)
{
    if (terms_len >= terms_cap) {
        size_t new_cap = terms_cap == 0 ? 16 : terms_cap * 2;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* Build the set of terms once. */
static void init_sequence(void)
{
    const size_t LIMIT = 1000000; /* covers the known maximum term (889 619) */

    /* ---------- Sieve of Eratosthenes ---------- */
    uint8_t *is_prime = (uint8_t *)calloc(LIMIT + 1, sizeof(uint8_t));
    if (!is_prime) abort();
    for (size_t i = 2; i <= LIMIT; ++i) is_prime[i] = 1;

    for (size_t p = 2; p * p <= LIMIT; ++p) {
        if (is_prime[p]) {
            for (size_t q = p * p; q <= LIMIT; q += p)
                is_prime[q] = 0;
        }
    }

    /* ---------- Prime counting function pi(x) ---------- */
    seq_t *pi = (seq_t *)malloc((LIMIT + 1) * sizeof(seq_t));
    if (!pi) abort();
    pi[0] = 0;
    for (size_t i = 1; i <= LIMIT; ++i)
        pi[i] = pi[i - 1] + (seq_t)is_prime[i];

    /* ---------- Precompute pi(d!) for digits 0..9 ---------- */
    seq_t fact[10];
    fact[0] = 1;
    for (int d = 1; d < 10; ++d) fact[d] = fact[d - 1] * (seq_t)d;

    seq_t pi_fact[10];
    for (int d = 0; d < 10; ++d) {
        /* factorial values are ≤ 9! = 362 880, safely inside LIMIT */
        pi_fact[d] = pi[(size_t)fact[d]];
    }

    /* ---------- Find all terms up to LIMIT ---------- */
    for (size_t n = 0; n <= LIMIT; ++n) {
        seq_t sum = 0;
        size_t tmp = n;
        if (tmp == 0) {
            sum += pi_fact[0];
        } else {
            while (tmp > 0) {
                int digit = tmp % 10;
                sum += pi_fact[digit];
                tmp /= 10;
            }
        }
        if (sum == pi[n])
            push_term((seq_t)n);
    }

    /* Clean up temporary storage */
    free(is_prime);
    free(pi);
    initialized = true;
}

/* Public function required by the problem description. */
seq_t a(idx_t n)
{
    if (!initialized)
        init_sequence();

    if (n == 0)
        return 0;                 /* undefined for 0, return 0 safely */

    size_t idx = (size_t)n - 1;    /* convert to 0‑based index */
    if (idx < terms_len)
        return terms[idx];
    else
        return 0;                 /* sequence has no further terms */
}
