/*
 * A080364  Composite numbers whose least prime factor appears with
 *          multiplicity 1.
 * https://oeis.org/A080364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th composite number whose least prime factor
 *            occurs exactly once in its factorisation. The implementation
 *            generates candidates in increasing order, skips primes and
 *            powers of the least prime factor, and caches the valid terms for
 *            fast random access
 * Range:     u8 n <= 117, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
 
/* Least prime factor of a 64‑bit integer.  For a prime the function
   returns the integer itself.  For small candidates a simple trial
   division is fast and sufficient for the sequence. */
static inline uint64_t lpf_uint64(uint64_t n)
{
    if ((n & 1ULL) == 0ULL) return 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL)
        if (n % d == 0ULL) return d;
    return n;                     /* n itself is prime */
}
 
/* Cache of computed terms.  The cache is extended lazily up to the
   required index whenever a() is called. */
static seq_t *cache   = NULL;  /* cache array */
static idx_t  size    = 0;     /* number of stored terms */
static idx_t  capacity = 0;    /* allocated size of cache array */
static idx_t  next    = 4;     /* next integer to test */
 
seq_t a(idx_t n)
{
    /* Ensure the cache contains at least n terms. */
    while (size < n)
    {
        /* Skip primes or numbers that are not composite. */
        uint64_t num   = (uint64_t)next;
        uint64_t lpf   = lpf_uint64(num);
        if (lpf == num)          /* prime */
        {
            ++next;
            continue;
        }
        /* Check that the least prime factor occurs exactly once. */
        uint64_t tmp = num;
        int exp = 0;
        while (tmp % lpf == 0ULL)
        {
            tmp /= lpf;
            ++exp;
            if (exp > 1) break;
        }
        if (exp != 1)
        {
            ++next;
            continue;           /* either exponent > 1 or 0 (impossible) */
        }
 
        /* Store the term.  Resize cache if necessary. */
        if (size + 1 > capacity)
        {
            capacity = capacity ? capacity * 2 : 16;
            seq_t *new_cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
            if (!new_cache) { /* abort on allocation failure */
                exit(1);
            }
            cache = new_cache;
        }
        cache[size++] = (seq_t)num;
        ++next;
    }
    return cache[n - 1];
}
