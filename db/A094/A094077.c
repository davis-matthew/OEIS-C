/*
 * A094077  a(1)=1 and, for n>1, a(n)=a(n-1)+n if n is even and a(n)=smallest
 *          positive integer which has not yet appeared in the sequence if n
 *          is odd.
 * https://oeis.org/A094077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1, if n is even, a(n)=a(n-1)+n; if n is odd, a(n) is the
   smallest positive integer not yet appearing in the sequence (mex). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static seq_t   *cache      = NULL;
static size_t   cache_cap  = 0;
static size_t   cache_len  = 0;

static unsigned char *used      = NULL;   /* used[i] == 1 if integer i has appeared */
static size_t         used_cap  = 0;      /* number of elements allocated in used */
static idx_t          next_candidate = 1; /* smallest unused integer */

static inline void ensure_cache_capacity(idx_t n)
{
    if (cache_len >= n) return;
    size_t required = (size_t)n;
    if (cache_cap < required)
    {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < required) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache     = tmp;
        cache_cap = new_cap;
    }
}

static inline void ensure_used_capacity(idx_t idx)
{
    if (used_cap > (size_t)idx) return;
    size_t new_cap = used_cap ? used_cap : 1;
    while (new_cap <= (size_t)idx) new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used, new_cap * sizeof(unsigned char));
    if (!tmp) abort();
    memset(tmp + used_cap, 0, new_cap - used_cap);
    used     = tmp;
    used_cap = new_cap;
}

static inline void advance_next_candidate(void)
{
    while (true)
    {
        if ((size_t)next_candidate >= used_cap)
            ensure_used_capacity((idx_t)next_candidate);
        if (!used[next_candidate]) break;
        ++next_candidate;
    }
}

static inline void mark_used(idx_t val)
{
    ensure_used_capacity(val);
    used[val] = 1;
    advance_next_candidate();
}

/* OEIS sequence A094077 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for n = 0 */

    if (cache_len == 0)               /* first call – initialise */
    {
        ensure_cache_capacity(1);
        cache[0] = (seq_t)1;          /* a(1) = 1 */
        cache_len = 1;
        mark_used(1);                 /* mark integer 1 as used */
    }

    if (n > cache_len)
    {
        ensure_cache_capacity(n);
        for (idx_t i = cache_len + 1; i <= n; ++i)
        {
            seq_t val;
            if ((i & 1) == 0)                 /* even i : a(i) = a(i-1) + i */
            {
                val = cache[i - 2] + (seq_t)i;
                cache[i - 1] = val;
                mark_used((idx_t)val);
            }
            else                               /* odd i : mex of previous values */
            {
                val = next_candidate;
                cache[i - 1] = val;
                mark_used((idx_t)val);
            }
        }
        cache_len = n;
    }

    return cache[n - 1];
}
