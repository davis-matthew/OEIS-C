/*
 * A068498  Fibonacci numbers whose sum of decimal digits is greater than its
 *          index.
 * https://oeis.org/A068498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 15, u64 n <= 29, u128 n <= 49,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A068498: Fibonacci numbers whose sum of decimal digits is greater than their index.
   a(n) = the n-th Fibonacci number F_k such that digit_sum(F_k) > k. */

#include <stdlib.h>
#include "../utils/digit_sum.h"

/* static cache of the sequence terms */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* state of the Fibonacci generator */
static seq_t fib_km2 = 0;   /* F_{k-2} */
static seq_t fib_km1 = 0;   /* F_{k-1} */
static idx_t fib_k = 0;     /* current Fibonacci index (k) */

/* grow the terms array if needed */
static void ensure_terms_cap(size_t need)
{
    if (need > terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 64;
        if (new_cap < 128) new_cap = 128;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }
}

/* initialise Fibonacci state and process k = 1 and k = 2 */
static void init_fib(void)
{
    fib_km2 = (seq_t)1;   /* F1 */
    fib_km1 = (seq_t)1;   /* F2 */
    fib_k   = 2;          /* last processed index */

    /* process k = 1 */
    if (digit_sum((seq_t)1) > (idx_t)1) {
        ensure_terms_cap(terms_len + 1);
        terms[terms_len++] = (seq_t)1;
    }

    /* process k = 2 */
    if (digit_sum((seq_t)1) > (idx_t)2) {
        ensure_terms_cap(terms_len + 1);
        terms[terms_len++] = (seq_t)1;
    }
}

/* extend the cache until it contains at least `target` terms */
static void ensure_terms(idx_t target)
{
    static int initialized = 0;
    if (!initialized) {
        initialized = 1;
        init_fib();
    }

    while ((idx_t)terms_len < target) {
        /* compute next Fibonacci number */
        seq_t next = fib_km1 + fib_km2;
        fib_km2 = fib_km1;
        fib_km1 = next;
        fib_k++;

        /* store the term if its digit sum exceeds its index */
        if (digit_sum(next) > fib_k) {
            ensure_terms_cap(terms_len + 1);
            terms[terms_len++] = next;
        }
    }
}

/* sequence entry point */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n = 0, return 0 as a safe value */
    ensure_terms(n);
    return terms[n - 1];
}
