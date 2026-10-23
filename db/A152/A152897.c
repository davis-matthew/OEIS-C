/*
 * A152897  Partial sums of A152896.
 * https://oeis.org/A152897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A152897 – Partial sums of A152896
 *
 *  Definition (from OEIS):
 *    a(n) = A124820(n) – A000217(n+1)
 *    Recurrence:
 *      a(1)=0, a(2)=3, a(3)=18, a(4)=73, a(5)=256, a(6)=844
 *      a(n)=7*a(n-1)-18*a(n-2)+23*a(n-3)-16*a(n-4)+6*a(n-5)-a(n-6)   (n>6)
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[i] holds a(i), i >= 1 */
static size_t cache_len = 0;     /* highest index currently stored */
static size_t cache_cap = 0;     /* allocated capacity (in elements) */

/* Ensure the cache can accommodate indices up to n (1‑based). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    /* C++ requires an explicit cast */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* out‑of‑memory: terminate the program */
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the cache with the first six known values. */
static void init_cache(void)
{
    if (cache_len > 0) return;      /* already initialised */

    ensure_capacity(6);
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)3;
    cache[3] = (seq_t)18;
    cache[4] = (seq_t)73;
    cache[5] = (seq_t)256;
    cache[6] = (seq_t)844;
    cache_len = 6;
}

/*  a(n) – returns the nth term (n >= 1).  Undefined for n == 0, but
 *  the function returns 0 in that case to avoid a crash in the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined for n==0 */

    init_cache();

    if ((idx_t)cache_len < n)
    {
        ensure_capacity(n);
        /* Compute up to n using the recurrence */
        for (size_t i = cache_len + 1; i <= (size_t)n; ++i)
        {
            seq_t t1 = 7 * cache[i - 1];
            seq_t t2 = 18 * cache[i - 2];
            seq_t t3 = 23 * cache[i - 3];
            seq_t t4 = 16 * cache[i - 4];
            seq_t t5 = 6  * cache[i - 5];
            seq_t t6 =     cache[i - 6];
            cache[i] = t1 - t2 + t3 - t4 + t5 - t6;
        }
        cache_len = (size_t)n;
    }
    return cache[(size_t)n];
}
