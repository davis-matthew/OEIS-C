/*
 * A087791  Indices n such that S^n(1) is an integer where S(x) =
 *          3/2*ceiling(x).
 * https://oeis.org/A087791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 41, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A087791 – Indices n such that Sⁿ(1) is an integer, where
   S(x) = (3/2) * ceil(x).  The sequence is indexed with offset 0:
   a(0) = 0, a(1) = 2, a(2) = 5, … */
#include <stdlib.h>
#include <stdint.h>

/* static state – cache of already found indices a(k) */
static idx_t *cache    = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* state of the recurrence */
static seq_t curr_m = (seq_t)1; /* m₀ = ceil(1) = 1 */
static idx_t next_k   = 1;      /* the k that will be tested next */
static int   initialized = 0;   /* one‑time initialisation flag */

/* ensure that the cache array has at least 'needed' slots */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
    if (!tmp) exit(1);            /* allocation failure – abort */
    cache = tmp;
    cache_cap = new_cap;
}

/* append a new index value to the cache */
static void cache_append(idx_t val)
{
    ensure_cache_cap(cache_len + 1);
    cache[cache_len++] = val;
}

/* ------------------------------------------------------------ */
/*    seq_t a(idx_t n)                                        */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* initialise on first invocation */
        cache_append((idx_t)0);    /* a(0) = 0 */
        curr_m   = (seq_t)1;       /* m₀ = 1  */
        next_k   = (idx_t)1;       /* first k to test */
        initialized = 1;
    }

    /* extend the cache until it contains a(n) */
    while (cache_len <= n) {
        /* x_k is integer iff m_{k‑1} is even */
        if ((curr_m & (seq_t)1) == (seq_t)0)
            cache_append(next_k);

        /* compute m_k = ceil(3/2 * m_{k‑1}) */
        seq_t next_m = (curr_m * (seq_t)3 + (curr_m & (seq_t)1)) / (seq_t)2;
        curr_m = next_m;
        ++next_k;
    }

    return (seq_t)cache[n];
}
