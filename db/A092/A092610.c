/*
 * A092610  Product of first n primes that end in 3.
 * https://oeis.org/A092610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) * p_n where p_n is the n‑th prime ending in 3,
 *            a(1) = 3. This function computes the product of the first n
 *            primes that end in 3, as required by OEIS A092610
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   grow_for_seq_t, next_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start); */
#include "../utils/seq_mul.h"      /* seq_t seq_mul(seq_t a, seq_t b); */
#include "../utils/grow_for_seq_t.h"

static seq_t *products = NULL;   /* cumulative products */
static size_t  prod_cap  = 0;    /* allocated capacity */
static size_t  prod_size = 0;    /* number of stored products */

static uint64_t next_start = 2;  /* next starting point for next_prime() */

/* Fill the products array up to index n (1‑based). */
static void ensure_n(idx_t n)
{
    if ((size_t)n <= prod_size)
        return;

    grow_for_seq_t((size_t)n, &prod_cap, &products);

    while (prod_size < (size_t)n) {
        /* Find the next prime after the current start point. */
        uint64_t p = next_prime(next_start);
        next_start = p + 1;          /* next search starts after p */

        /* If p ends in 3 we include it in the product sequence. */
        if (p % 10 == 3) {
            if (prod_size == 0) {
                /* First product is just the first prime ending in 3. */
                products[prod_size++] = (seq_t)p;
            } else {
                /* Multiply the previous product by the new prime. */
                seq_t prod = seq_mul(products[prod_size - 1], (seq_t)p);
                products[prod_size++] = prod;
            }
        }
    }
}

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term. For safety return 0 for n==0. */
    if (n == 0)
        return (seq_t)0;

    ensure_n(n);
    return products[(size_t)n - 1];
}
