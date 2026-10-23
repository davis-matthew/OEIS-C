/*
 * A129761  First differences of Fibbinary numbers (A003714).
 * https://oeis.org/A129761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibbinary(n+1) – Fibbinary(n), where a Fibbinary number
 *            is an integer with no consecutive 1s in binary
 * Range:     u8 n <= 87, u16 n <= 6763, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static cache of the first few Fibbinary numbers. */
static seq_t *fib_cache      = NULL;   /* array of Fibbinary numbers   */
static size_t fib_capacity  = 0;       /* capacity of fib_cache       */
static size_t fib_size      = 0;       /* number of elements stored   */
static seq_t next_candidate = 1;       /* next integer to test       */

/* Ensure the cache is initialized. */
static void init_cache(void)
{
    if (fib_cache == NULL) {
        fib_capacity = 128;
        fib_cache = (seq_t *)malloc(fib_capacity * sizeof(seq_t));
        fib_cache[0] = (seq_t)0;          /* Fibbinary(0) = 0           */
        fib_size   = 1;
        next_candidate = (seq_t)1;
    }
}

/* Append a new Fibbinary number to the cache, growing if necessary. */
static void append_fibbinary(seq_t val)
{
    if (fib_size == fib_capacity) {
        fib_capacity *= 2;
        fib_cache = (seq_t *)realloc(fib_cache, fib_capacity * sizeof(seq_t));
    }
    fib_cache[fib_size++] = val;
}

/* Generate Fibbinary numbers until at least the (n+1)-th element
   is available in the cache. */
static void ensure_fib_upto(idx_t n)
{
    /* n may be large; we only need to fill to index n+1. */
    size_t needed = (size_t)(n + 2);  /* because we want fib[n+1] */
    if (fib_size >= needed) {
        return;
    }

    /* Continue searching from the last tested integer. */
    while (fib_size < needed) {
        seq_t x = next_candidate;
        /* Test if x has no consecutive ones: (x & (x<<1)) == 0. */
        if ((x & (x << 1)) == (seq_t)0) {
            append_fibbinary(x);
        }
        ++next_candidate;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    init_cache();
    ensure_fib_upto(n);              /* ensure fib[n] and fib[n+1] exist */
    return fib_cache[(size_t)(n + 1)] - fib_cache[(size_t)n];
}
