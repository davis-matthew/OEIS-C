/*
 * A164348  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A164348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   For n > 6: a(n) = -1081*a(n-6) + 46*∑_{k=1..5} a(n-k) For n = 6
 *            additionally a(6) = 1 + [above formula], coming from the
 *            numerator term of the generating function
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS sequence A164348. */
#include <stddef.h>
#include <stdlib.h>

/* Helper array with the first six terms (offset 0). */
static const seq_t init[6] = {
    (seq_t)1,          /* a(0) */
    (seq_t)48,         /* a(1) */
    (seq_t)2256,       /* a(2) */
    (seq_t)106032,     /* a(3) */
    (seq_t)4983504,    /* a(4) */
    (seq_t)234224688   /* a(5) */
};

/* Static cache for previously computed values. */
static seq_t *cache = NULL;
static size_t  cap   = 0;   /* allocated size of cache */
static size_t  sz    = 0;   /* number of terms actually stored */

/* Ensure the cache has room for at least `need` elements. */
static void
ensure_cache(size_t need)
{
    if (need <= cap) return;
    /* Allocate a new buffer of the requested size. */
    seq_t *tmp = NULL;
    if (cache == NULL) {
        tmp = (seq_t *)malloc(need * sizeof(seq_t));
    } else {
        tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
    }
    if (tmp == NULL) {
        /* In a real program we would handle the error; here we just
           abort as the caller cannot recover. */
        abort();
    }
    cache = tmp;
    cap   = need;
}

/* Initialise the cache with the first six terms when it is empty. */
static void
init_cache(void)
{
    if (sz == 0) {
        /* Make sure we have space for at least six elements. */
        if (cap < 6) {
            ensure_cache(6);
        }
        for (size_t i = 0; i < 6; ++i) {
            cache[i] = init[i];
        }
        sz = 6;
    }
}

seq_t a(idx_t n)
{
    if (n < 6) {
        return init[(size_t)n];
    }

    /* Allocate/resize the cache if necessary. */
    ensure_cache((size_t)n + 1);

    /* Initialise the cache with the starting values if it has not been
       done yet. */
    init_cache();

    /* Compute new terms until we reach a(n). */
    while (sz <= (size_t)n) {
        /* Sum of the five preceding terms. */
        seq_t sum =
            cache[sz - 1] + cache[sz - 2] + cache[sz - 3] +
            cache[sz - 4] + cache[sz - 5];

        /* Apply the recurrence. */
        seq_t val = 46 * sum - (seq_t)1081 * cache[sz - 6];

        /* The numerator contributes +1 only for n = 6. */
        if (sz == 6) {
            val += (seq_t)1;
        }

        cache[sz] = val;
        ++sz;
    }

    return cache[(size_t)n];
}
