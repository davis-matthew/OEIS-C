/*
 * A204477  Number of (n+2)X4 0..3 arrays with every 3X3 subblock having three
 *          equal elements in a row horizontally, vertically or nw-to-se
 *          diagonally exactly three ways, and new values 0..3 introduced in
 *          row major order.
 * https://oeis.org/A204477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 2*a(n-3) - 19*a(n-4) - 4*a(n-5) +
 *            21*a(n-6) + 24*a(n-7) + 3*a(n-8) - 18*a(n-9) - 9*a(n-10) Initial
 *            values (n=1..13): 307, 1326, 3147, 2745, 8114, 29959, 111707,
 *            425010, 1602835, 6076455, 23038228, 87322215, 331059861
 * Range:     u8 none, u16 n <= 6, u32 n <= 14, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* 1‑based storage: cache[i-1] = a(i) */
static size_t cache_len = 0;         /* number of terms currently stored (>=1) */
static size_t cache_cap = 0;         /* allocated capacity */

/* First 13 terms of the sequence */
static const seq_t init_vals[13] = {
    307ULL, 1326ULL, 3147ULL, 2745ULL, 8114ULL, 29959ULL, 111707ULL,
    425010ULL, 1602835ULL, 6076455ULL, 23038228ULL, 87322215ULL, 331059861ULL
};

/* Ensure we have room for at least n terms */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < (size_t)n) new_cap <<= 1;
    seq_t *new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();            /* fatal allocation error */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Initialise cache with the first 13 terms */
static void init_cache(void)
{
    if (cache != NULL) return;
    ensure_capacity(14);              /* room for the first 13 entries */
    for (size_t i = 0; i < 13; ++i) cache[i] = init_vals[i];
    cache_len = 13;
}

/* Compute terms up to n (inclusive) using the recurrence */
static void compute_upto(idx_t n)
{
    if ((size_t)n <= cache_len) return;
    for (idx_t k = cache_len + 1; k <= n; ++k)
    {
        /* a(k) depends on a(k-1) … a(k-10) */
        seq_t val =
            4 * cache[k-2]          /* 4*a(k-1)     */
          + cache[k-3]              /* +a(k-2)      */
          - 2 * cache[k-4]          /* -2*a(k-3)    */
          - 19 * cache[k-5]         /* -19*a(k-4)   */
          - 4 * cache[k-6]          /* -4*a(k-5)    */
          + 21 * cache[k-7]         /* +21*a(k-6)   */
          + 24 * cache[k-8]         /* +24*a(k-7)   */
          + 3 * cache[k-9]          /* +3*a(k-8)    */
          - 18 * cache[k-10]        /* -18*a(k-9)   */
          - 9 * cache[k-11];        /* -9*a(k-10)   */
        cache[k-1] = val;
    }
    cache_len = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* values below the offset are undefined */
    init_cache();
    ensure_capacity(n);
    compute_upto(n);
    return cache[n-1];
}
