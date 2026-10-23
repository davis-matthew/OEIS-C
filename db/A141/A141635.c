/*
 * A141635  Odd nonprimes of the form j/(p(j+1)-p(j)), where p(j)=j-th prime.
 * https://oeis.org/A141635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime_small, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A141635 – odd non‑prime values of  j /(p(j+1)-p(j)),
    where p(j) is the j‑th prime.
    a(1) is the first term, a(n) is produced in order of increasing j. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/next_prime.h"    /* uint64_t next_prime(uint64_t start); */
#include "../utils/is_prime_small.h"/* bool is_prime_small(seq_t n); */

/* Cache for the sequence */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Variables for generating primes on the fly */
static bool primes_inited = false;
static uint64_t prev_p = 0;   /* p(j)   */
static uint64_t curr_p = 0;   /* p(j+1) */
static idx_t next_j = 1;      /* j to process next */

/* Grow the cache if needed */
static void cache_grow(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t cap = cache_cap ? cache_cap : 8;
    while (cap < new_cap)
        cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(*cache));
    cache = tmp;
    cache_cap = cap;
}

/* Append a value to the cache */
static void cache_append(seq_t val)
{
    cache_grow(cache_len + 1);
    cache[cache_len++] = val;
}

/* Initialize the first two primes p(1)=2 and p(2)=next_prime(2) */
static void init_primes(void)
{
    if (primes_inited)
        return;
    prev_p = 2ULL;
    curr_p = next_prime(prev_p);
    primes_inited = true;
}

/*  a(idx_t n) – compute the n-th term (1‑based) */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset 1 – return 0 for invalid index */
        return 0;

    init_primes();

    /* Generate terms until we have at least n cached */
    while (cache_len < (size_t)n) {
        idx_t j = next_j;
        uint64_t gap = curr_p - prev_p;        /* p(j+1)-p(j) */

        if (gap != 0 && j % gap == 0) {
            idx_t r = j / gap;                 /* integer quotient */
            seq_t val = (seq_t)r;
            if ((val & 1U) && !is_prime_small(val))
                cache_append(val);
        }

        /* Advance to next j: shift primes */
        prev_p = curr_p;
        curr_p = next_prime(curr_p);
        ++next_j;
    }

    return cache[n - 1];
}
