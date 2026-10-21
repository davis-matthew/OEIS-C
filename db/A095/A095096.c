/*
 * A095096  Fibevil numbers: those numbers k for which the parity of 1-fibits
 *          in Zeckendorf expansion A014417(k) is even, i.e., for which
 *          A095076(k) = 0.
 * https://oeis.org/A095096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th number whose Zeckendorf representation (using
 *            Fibonacci numbers 1,2,3,5,8,13,...) has an even number of 1's
 * Range:     u8 n <= 128, u16 n <= 8764, u32 n <= 8764, u64 n <= 8764,
 *            u128 n <= 8764, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Local helpers for Fibonacci numbers used in Zeckendorf
   representation.  The array grows as needed. */
static seq_t *fib = NULL;
static size_t fib_cnt = 0;      /* number of fibs stored   */
static size_t fib_cap = 0;      /* allocated capacity     */

/* Initialize the Fibonacci array with 1,2. */
static void init_fib_array(void)
{
    if (fib_cnt) return;
    fib_cap = 4;
    fib = (seq_t *)malloc(fib_cap * sizeof(seq_t));
    fib[0] = (seq_t)1;
    fib[1] = (seq_t)2;
    fib_cnt = 2;
}

/* Extend the Fibonacci array up to at least 'limit'. */
static void extend_fib_until(seq_t limit)
{
    init_fib_array();
    while (fib[fib_cnt - 1] < limit) {
        /* Compute next Fibonacci number using a wide intermediate type.
           An overflow in seq_t indicates that we cannot track further
           Fibonacci numbers (which happens only if limit is beyond the
           representable range). */
        unsigned long long next = (unsigned long long)fib[fib_cnt - 1] +
                                   (unsigned long long)fib[fib_cnt - 2];
        if (next > (unsigned long long)limit) break;
        if (next < (unsigned long long)fib[fib_cnt - 1]) break; /* overflow */

        if (fib_cnt == fib_cap) {
            fib_cap = fib_cap ? fib_cap * 2 : 4;
            fib = (seq_t *)realloc(fib, fib_cap * sizeof(seq_t));
        }
        fib[fib_cnt++] = (seq_t)next;
    }
}

/* Return true if the Zeckendorf representation of 'n' contains an even
   number of 1's. */
static bool has_even_zeckendorf_parity(seq_t n)
{
    if (n == 0) return true;   /* 0 has zero 1’s – even */
    extend_fib_until(n);

    /* Find the largest Fibonacci <= n */
    int i = (int)fib_cnt - 1;
    bool parity = false; /* false => even, true => odd */

    seq_t rest = n;
    while (rest > 0) {
        while (fib[i] > rest) --i;
        parity = !parity;              /* toggle for each 1-bit */
        rest -= fib[i];
    }
    return !parity; /* return true if even */
}

/* Cache of the sequence values. */
static seq_t *cache = NULL;
static size_t cache_sz  = 0;  /* number of cached elements */
static size_t cache_cap = 0;

/* Ensure the cache has at least 'need' elements.  New elements are
   appended until the required index is reached. */
static void ensure_cache_size(idx_t need)
{
    if (need <= cache_sz) return;

    /* Start from 0 if cache empty. */
    seq_t candidate = 0;
    if (cache_sz > 0) candidate = cache[cache_sz - 1] + 1;

    for (; cache_sz < need; ++candidate) {
        if (has_even_zeckendorf_parity(candidate)) {
            if (cache_sz == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 8;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_sz++] = candidate;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* out of bounds, but harmless */
    ensure_cache_size(n);
    return cache[n - 1];
}
