/*
 * A119884  Expansion of e.g.f. sech(x)/(1-x).
 * https://oeis.org/A119884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A119884 – expansion of e.g.f. sech(x)/(1‑x)
   Formula (recurrence): 
     a(0) = 1
     a(n) = Σ_{k=0..n-1} binomial(n,k) * a(k) * b(n‑k)
     where b(m) = m   for odd m,
           b(m) = –1  for even m.
   The implementation saves already computed values in a static
   dynamic array so that calls in arbitrary order stay O(n).  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* the types seq_t and idx_t are supplied by the build system */
extern void *malloc(size_t);
extern void *realloc(void *, size_t);
extern void free(void *);

/* ------------------------------------------------------------ */
/*  Utility: binomial coefficient  C(a,b)  (0 ≤ b ≤ a)          */
/* ------------------------------------------------------------ */
static seq_t
binomial(idx_t a, idx_t b)
{
    if (b > a) return (seq_t)0;
    if (b == 0 || b == a) return (seq_t)1;
    if (b > a - b) b = a - b;          /* use symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= b; ++i) {
        /* multiply by (a - b + i) then divide by i */
        result = result * (seq_t)(a - b + i) / (seq_t)i;
    }
    return result;
}

/* ------------------------------------------------------------ */
/*  Dynamic cache for sequence values                           */
/* ------------------------------------------------------------ */
static seq_t *cache      = NULL;   /* array holding a(0)…a(n) */
static idx_t  cache_len  = 0;      /* number of stored terms   */
static idx_t  cache_cap  = 0;      /* allocated capacity */

/* Ensure that cache can address indices 0…n */
static void
ensure_cache(idx_t n)
{
    if (cache_cap > n) return;              /* already enough space */
    idx_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* realloc with NULL behaves like malloc; if it fails, the program
       will abort – acceptable for this context. */
    cache = new_cache;
    cache_cap = new_cap;
    /* cache_len stays unchanged – only newly allocated items are
       uninitialised */
}

/* ------------------------------------------------------------ */
/*  Main sequence function                                      */
/* ------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    /* grow cache to accommodate index n */
    ensure_cache(n);

    /* compute all missing values up to n */
    if (cache_len <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }
            seq_t sum = 0;
            for (idx_t k = 0; k < i; ++k) {
                seq_t c = binomial(i, k);
                seq_t term = c * cache[k];
                idx_t m = i - k;
                if ((m & 1) == 0) {          /* even → b(m) = –1 */
                    sum -= term;
                } else {                     /* odd  → b(m) = m   */
                    sum += term * (seq_t)m;
                }
            }
            cache[i] = sum;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
