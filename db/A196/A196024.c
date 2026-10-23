/*
 * A196024  Odious Fibonacci numbers.
 * https://oeis.org/A196024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS A196024 sequence (Odious Fibonacci numbers).
   Then a(n) is the n-th Fibonacci number whose binary expansion has an
   odd number of 1‑bits (i.e. an odious number). The implementation
   merely generates Fibonacci numbers in order, keeps those that are
   odious, and memoises them. */

#include <stdlib.h>
#include <stdbool.h>

/* Test whether a seq_t value is odious (odd popcount). */
static bool is_odious(seq_t x)
{
    bool parity = false;                /* false = even, true = odd   */
    while (x != (seq_t)0) {
        parity = !parity;
        x = x & (x - (seq_t)1);         /* drop lowest 1‑bit */
    }
    return parity;
}

/* Simple dynamic array for the odious Fibonacci cache. */
static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* Ensure the cache has room for at least one new element. */
static void cache_ensure(void)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) exit(EXIT_FAILURE);
        cache = newp;
        cache_cap = new_cap;
    }
}

/* Append a new value to the cache. */
static void cache_append(seq_t val)
{
    cache_ensure();
    cache[cache_size++] = val;
}

/* Fibonacci state maintained between calls. */
static seq_t f_prev = (seq_t)1;   /* F1 */
static seq_t f_cur  = (seq_t)1;   /* F2 */
static bool   initialized = false;

/* Generate odious Fibonacci numbers until we have at least n. */
static void ensure_up_to(idx_t n)
{
    if (!initialized) {
        /* First odious Fibonacci is 1 (F1).  F2 is also 1 but
           we count only distinct Fibonacci numbers, so we store it once. */
        if (is_odious(f_prev)) cache_append(f_prev);
        initialized = true;
    }

    while (cache_size < n) {
        seq_t next = f_prev + f_cur;  /* F(k+1) = F(k-1)+F(k) */
        f_prev = f_cur;
        f_cur  = next;
        if (is_odious(f_cur)) cache_append(f_cur);
    }
}

/* Sequence A196024: a(n) = the n-th odious Fibonacci number. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1 */
    ensure_up_to(n);
    return cache[n - 1];
}
