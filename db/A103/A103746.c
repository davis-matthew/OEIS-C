/*
 * A103746  Numbers n such that prime(n)*prime(n+1) - 1 is semiprime.
 * https://oeis.org/A103746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A103746:
 * a(n) = the nth integer k such that prime(k)*prime(k+1)-1
 * is a semiprime (product of exactly two primes, counting multiplicities). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* --------------------- Prime management --------------------- */
static uint64_t       *prime_tbl   = NULL;   /* array of primes (1‑based indexing) */
static idx_t          prime_cnt    = 0;      /* number of primes stored */
static size_t         prime_cap    = 0;      /* current capacity of prime_tbl */

/* ensure prime_tbl has at least 'new_cap' slots */
static void grow_prime_tbl(size_t new_cap) {
    uint64_t *tmp = (uint64_t *)realloc(prime_tbl, new_cap * sizeof(uint64_t));
    if (!tmp) exit(1);
    prime_tbl = tmp;
    prime_cap = new_cap;
}

/* add the next prime to prime_tbl */
static void generate_next_prime(void) {
    static uint64_t last = 1;                 /* last prime added */
    uint64_t cand = last + 1;
    if (cand < 2) cand = 2;
    while (!is_prime(cand)) ++cand;
    if (prime_cnt == prime_cap) grow_prime_tbl(prime_cap ? prime_cap * 2 : 16);
    prime_tbl[prime_cnt++] = cand;
    last = cand;
}

/* guarantee we have primes up to the index 'idx' (1‑based) */
static void ensure_primes_up_to_index(idx_t idx) {
    while (prime_cnt <= idx) generate_next_prime();
}

/* --------------------- Semiprime test --------------------- */
static bool is_semiprime(seq_t x) {
    if (x <= (seq_t)1) return false;
    idx_t  count = 0;
    seq_t  tmp   = x;

    /* ensure we have primes up to sqrt(tmp) */
    while (prime_cnt == 0 ||
           ((seq_t)prime_tbl[prime_cnt-1] * (seq_t)prime_tbl[prime_cnt-1] <= tmp))
        generate_next_prime();

    for (idx_t i = 0; i < prime_cnt; ++i) {
        seq_t p = (seq_t)prime_tbl[i];
        if (p * p > tmp) break;
        while (tmp % p == 0) {
            ++count;
            if (count > 2) return false;
            tmp /= p;
        }
    }
    if (tmp > (seq_t)1) ++count;
    return count == 2;
}

/* --------------------- Sequence caching --------------------- */
static idx_t *terms_tbl = NULL;   /* store indices k of the sequence */
static idx_t  terms_cnt = 0;      /* number of stored terms      */
static size_t terms_cap = 0;      /* capacity of terms_tbl       */

static void grow_terms_tbl(void) {
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    idx_t *tmp = (idx_t *)realloc(terms_tbl, new_cap * sizeof(idx_t));
    if (!tmp) exit(1);
    terms_tbl = tmp;
    terms_cap = new_cap;
}

static idx_t last_checked_k = 0; /* largest k examined so far */

/* --------------------- Main function --------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* out‑of‑range, avoid crash */

    /* return cached value if already computed */
    if (terms_cnt >= n) return (seq_t)terms_tbl[n-1];

    /* generate terms until the n-th is found */
    while (terms_cnt < n) {
        ++last_checked_k;                     /* candidate k */

        /* ensure we have primes p_k and p_{k+1} */
        ensure_primes_up_to_index(last_checked_k + 1);

        seq_t prod =
            (seq_t)prime_tbl[last_checked_k-1] *
            (seq_t)prime_tbl[last_checked_k] - (seq_t)1;

        if (is_semiprime(prod)) {
            if (terms_cnt == terms_cap) grow_terms_tbl();
            terms_tbl[terms_cnt++] = last_checked_k;
        }
    }

    return (seq_t)terms_tbl[n-1];
}
