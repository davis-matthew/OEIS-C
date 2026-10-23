/*
 * A111072  Write the digit string 0123456789, repeated infinitely many times.
 *          Then, starting from the first "0" digit at the left end, move to
 *          the right by one digit (to the "1"), then two digits (to the "3"),
 *          then three digits (to the "6"), four digits ("0"), five digits
 *          ("5"), and so on. Partial sums of t...
 * https://oeis.org/A111072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=0; for n≥1, a(n)=a(n-1)+(( (n-1)*n/2 ) mod 10)
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The n‑th digit reached after a jumps of lengths 1..n is ((n*(n+1)/2) mod 10). */

#include <stdlib.h>

/* Helpers for memory allocation that work in both C and C++.
   In C++ use new/delete[]; in C use malloc/free. */
#ifdef __cplusplus
#  include <new>
static seq_t* allocate(idx_t size) { return new seq_t[size]; }
static void deallocate(seq_t* p, idx_t /*size*/) { delete[] p; }
#else
static seq_t* allocate(idx_t size) { return (seq_t*)malloc(size * sizeof(seq_t)); }
static void deallocate(seq_t* p, idx_t /*size*/) { free(p); }
#endif

/* O(1) cache for previously computed values. */
static seq_t *cache   = NULL;   /* array of a(n) values     */
static idx_t  cap    = 0;       /* capacity of cache       */
static idx_t  len    = 0;       /* number of entries stored */

/* Compute the n‑th digit reached: ((n*(n+1)/2) mod 10). */
static inline seq_t digit_at(idx_t n)
{
    /* Compute n*(n+1)/2 without overflow. */
    seq_t a = (seq_t)n;
    seq_t b = (seq_t)(n + 1);
    if ((n & 1) == 0)          /* n is even, divide a by 2 */
        a /= 2;
    else                       /* n is odd, divide b by 2 */
        b /= 2;
    seq_t da = a % 10;
    seq_t db = b % 10;
    return (da * db) % 10;     /* final digit */
}

/* Grow the cache so that it can hold indices up to (needed-1). */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cap) return;

    seq_t *new_cache = allocate(needed);
    /* Copy existing values. */
    for (idx_t i = 0; i < len; ++i)
        new_cache[i] = cache[i];
    /* Free old buffer if any. */
    if (cache)
        deallocate(cache, cap);
    cache = new_cache;
    cap   = needed;
}

/* Public function required by the judge. */
seq_t a(idx_t n)
{
    /* Initialise cache on first call. */
    if (len == 0) {
        ensure_capacity(16);
        cache[0] = 0;          /* a(0) = 0 */
        len = 1;
    }

    /* Extend cache up to index n. */
    if (n >= len) {
        ensure_capacity(n + 1);
        for (idx_t i = len; i <= n; ++i)
            cache[i] = cache[i - 1] + digit_at(i);
        len = n + 1;
    }

    return cache[n];
}
