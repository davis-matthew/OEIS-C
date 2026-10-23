/*
 * A155011  Sophie Germain Fibonacci prime numbers.
 * https://oeis.org/A155011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A263880(n) - 1)/2 where A263880(n) = 2*p+1 with p a
 *            Fibonacci prime. This function generates Fibonacci numbers,
 *            tests each one for primality, and also checks that 2*p+1 is
 *            prime. The Fibonacci primes p that satisfy this property are
 *            stored in a growing cache and returned on demand
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   alloc_seq, free_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"
#include "../utils/is_prime_big.h"

/* Cache of found terms a(1)..a(cache_len) */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Current Fibonacci state (iterative generation) */
static seq_t fib_prev = 1;  /* F(1)   */
static seq_t fib_cur  = 1;  /* F(2)   */
static idx_t fib_index = 2; /* index of fib_cur */

/* Ensure cache can hold at least 'need' elements */
static void ensure_cache_size(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;

    seq_t *new_arr = alloc_seq(new_cap);
    for (size_t i = 0; i < cache_len; ++i)
        new_arr[i] = cache[i];
    if (cache != NULL)
        free_seq(cache);
    cache = new_arr;
    cache_cap = new_cap;
}

/* Generate Fibonacci numbers until 'n_needed' terms are found */
static void generate_terms_until(size_t n_needed)
{
    while (cache_len < n_needed) {
        /* Next Fibonacci number: F(k) = F(k-1) + F(k-2) */
        fib_index++;                   /* move to index k          */
        seq_t fib_next = fib_prev + fib_cur;
        fib_prev = fib_cur;
        fib_cur  = fib_next;

        /* Test primality of fib_next and of 2*fib_next+1 */
        if (is_prime_big(fib_next)) {
            seq_t q = ((seq_t)2 * fib_next) + 1;
            if (is_prime_big(q)) {
                /* Found a new Sophie Germain Fibonacci prime */
                ensure_cache_size(cache_len + 1);
                cache[cache_len++] = fib_next;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined; guard against underflow */

    size_t n_needed = (size_t)n;
    if (cache_len < n_needed)
        generate_terms_until(n_needed);
    return cache[n_needed - 1];
}
