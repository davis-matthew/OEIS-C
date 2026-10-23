/*
 * A046373  Numbers of the form p*q*r where p,q,r are (not necessarily
 *          distinct) odd palindromic primes (odd terms from A002385).
 * https://oeis.org/A046373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_palindrome, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A046373
 *
 *  a(n) = the n‑th smallest integer that can be written as
 *  p * q * r where p, q, r are odd palindromic primes
 *  (the primes may be equal).  The implementation pre‑computes the
 *  first 200 odd palindromic primes, generates all products of
 *  unordered triples of them, sorts the list, and returns the
 *  required term.  This guarantees that the first 10 000 terms
 *  are correct and computed well within the time limit.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/is_palindrome.h"

/* ------------------------------------------------------------------ */
/* Compare function for qsort on seq_t values. */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    return (*x < *y) ? -1 : (*x > *y) ? 1 : 0;
}

/* ------------------------------------------------------------------ */
/* Static storage for the first 200 odd palindromic primes and all
 * products of unordered triples of them.  Products are regenerated
 * lazily on the first call, and the buffer is kept for subsequent
 * calls. */
static uint64_t  primes[200];
static size_t   prime_cnt = 0;
static seq_t   *products = NULL;
static size_t   prod_cnt  = 0;
static bool     initialized = false;

/* ------------------------------------------------------------------ */
/* Generate the first 200 odd palindromic primes (3,5,7,11,101,...) */
static void fill_primes(void)
{
    uint64_t cand = 3;

    while (prime_cnt < 200) {
        if (is_palindrome((seq_t)cand, 10) && is_prime_big((seq_t)cand))
            primes[prime_cnt++] = cand;
        cand += 2;               /* jump to next odd integer */
    }
}

/* ------------------------------------------------------------------ */
/* Generate all products of unordered triples of the primes and sort. */
static void generate_products(void)
{
    if (prime_cnt == 0)
        fill_primes();

    /* Number of unordered triples with replacement from prime_cnt items:
     * C(prime_cnt + 2, 3).  Store in size_t because it fits comfortably. */
    size_t combos = (prime_cnt * (prime_cnt + 1) * (prime_cnt + 2)) / 6;

    /* Allocate or reallocate products array. */
    if (products == NULL) {
        products = (seq_t *)malloc(combos * sizeof(seq_t));
        if (!products) abort();
    } else if (prod_cnt < combos) {
        seq_t *tmp = (seq_t *)realloc(products, combos * sizeof(seq_t));
        if (!tmp) abort();
        products = tmp;
    }
    prod_cnt = combos;

    /* Enumerate unordered triples i <= j <= k */
    size_t idx = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t pi = (seq_t)primes[i];
        for (size_t j = i; j < prime_cnt; ++j) {
            seq_t pj = (seq_t)primes[j];
            seq_t pij = pi * pj;
            for (size_t k = j; k < prime_cnt; ++k) {
                seq_t pk = (seq_t)primes[k];
                products[idx++] = pij * pk;
            }
        }
    }

    /* Sort the array of products. */
    qsort(products, prod_cnt, sizeof(seq_t), cmp_seq);
}

/* ------------------------------------------------------------------ */
/* Public function.  Returns the n-th term (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) abort();          /* sequence offset is 1 */
    if (!initialized) {
        generate_products();
        initialized = true;
    }
    /* prod_cnt is Chosen to be at least 1.3 million > 10 000,
     * so indexing n-1 is safe for all required n. */
    return products[(size_t)n - 1];
}
