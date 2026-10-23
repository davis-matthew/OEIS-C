/*
 * A152500  1/4 the number of permutations of 3 indistinguishable copies of
 *          1..n with exactly 3 local maxima.
 * https://oeis.org/A152500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 108*a(n-1) - 3840*a(n-2) + 58752*a(n-3) * - 401664*a(n-4)
 *            + 1244160*a(n-5) - 1433600*a(n-6) * Initial values: * a(1)=0,
 *            a(2)=1, a(3)=231, a(4)=21490, a(5)=1476084, a(6)=90050080 * The
 *            function memoises results to allow out‑of‑order calls. * * This
 *            implementation uses only the operators all...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Multiply seq_t by an unsigned constant.
 * For primitive types this uses normal integer multiplication
 * (overflow modulo the type width).  For big‑integer types
 * the overloaded operator* provides the exact product. */
static inline seq_t mul_uint(seq_t x, unsigned int k)
{
    return x * (seq_t)k;
}

/* Lazy cache for memoised values.
 * cache[i] holds a(i+1).  It grows only as needed. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of filled entries (max computed n) */
static size_t cache_cap = 0;   /* allocated capacity */

/* Ensure the cache can hold at least 'needed' entries. */
static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = (cache_cap > 0) ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* allocation failure */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a(n).  Sequence is memoised; calls may arrive in any order. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* base case for indices below the offset */

    size_t needed = (size_t)n;          /* indices 0 .. n-1 */
    ensure_capacity(needed);

    /* Populate cache up to n */
    for (size_t i = cache_len; i < needed; ++i) {
        seq_t val;
        if (i == 0)                val = (seq_t)0;          /* a(1) */
        else if (i == 1)           val = (seq_t)1;          /* a(2) */
        else if (i == 2)           val = (seq_t)231;        /* a(3) */
        else if (i == 3)           val = (seq_t)21490;      /* a(4) */
        else if (i == 4)           val = (seq_t)1476084;    /* a(5) */
        else if (i == 5)           val = (seq_t)90050080;   /* a(6) */
        else {
            /* a(n) for n > 6 depends on the previous six values */
            seq_t a1 = cache[i - 1]; /* a(n-1) */
            seq_t a2 = cache[i - 2]; /* a(n-2) */
            seq_t a3 = cache[i - 3]; /* a(n-3) */
            seq_t a4 = cache[i - 4]; /* a(n-4) */
            seq_t a5 = cache[i - 5]; /* a(n-5) */
            seq_t a6 = cache[i - 6]; /* a(n-6) */

            seq_t pos = mul_uint(a1, 108);
            pos += mul_uint(a3, 58752);
            pos += mul_uint(a5, 1244160);

            seq_t neg = mul_uint(a2, 3840);
            neg += mul_uint(a4, 401664);
            neg += mul_uint(a6, 1433600);

            val = pos - neg; /* subtraction works modulo the type width */
        }
        cache[i] = val;
    }
    cache_len = needed;
    return cache[n - 1];
}
