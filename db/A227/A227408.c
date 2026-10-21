/*
 * A227408  Set of all n, where n = r(s(n)) = s(r(n)), given that r(n) =
 *          n+bitcount(n), s(n) = n-bitcount(n), and bitcount(n) is the count
 *          of binary 1's in n.
 * https://oeis.org/A227408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 3574, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A227408: numbers n with r(s(n)) = s(r(n))
   where r(n)=n+popcount(n), s(n)=n-popcount(n).
   The function generates terms by testing the condition and uses
   a static cache to answer queries in any order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* popcount for an arbitrary integer type seq_t */
static inline seq_t popcount_seq(seq_t x)
{
    seq_t cnt = 0;
    while (x != (seq_t)0) {
        x &= x - (seq_t)1;
        ++cnt;
    }
    return cnt;
}

/* static cache for the sequence */
static seq_t *cache = NULL;
static size_t  cap   = 0;
static size_t  count = 0;        /* number of terms stored */
static seq_t  next = 1;          /* next candidate to test */

/* ensure capacity of the cache */
static inline void ensure_cap(void)
{
    if (count >= cap) {
        size_t newcap = (cap == 0) ? 16 : cap * 2;
#ifdef __cplusplus
        seq_t *tmp = reinterpret_cast<seq_t*>(realloc(cache, newcap * sizeof(seq_t)));
#else
        seq_t *tmp = realloc(cache, newcap * sizeof(seq_t));
#endif
        if (!tmp) abort();
        cache = tmp;
        cap   = newcap;
    }
}

/* append a value to the cache */
static inline void cache_append(seq_t v)
{
    ensure_cap();
    cache[count++] = v;
}

/* the sequence function */
seq_t a(idx_t n)
{
    /* initialise cache with a(1)=0 */
    if (count == 0) {
        cache_append((seq_t)0);
    }

    /* generate terms until we have at least n terms */
    while (count < n) {
        seq_t  x = next++;
        seq_t  b = popcount_seq(x);
        seq_t  nm = x - b;
        seq_t  np = x + b;
        if (popcount_seq(nm) == b && popcount_seq(np) == b) {
            cache_append(x);
        }
    }
    return cache[n - 1];
}
