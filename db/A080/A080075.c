/*
 * A080075  Proth numbers: of the form k*2^m + 1 for k odd, m >= 1 and 2^m >
 *          k.
 * https://oeis.org/A080075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 3 Recurrence: a(n) = a(n-1) + 2^round(L(a(n-1))/2), n >=
 *            2 where L(x) is the number of binary digits of x
 * Range:     u8 n <= 22, u16 n <= 382, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses dynamic caching; works for all supported seq_t types. */
#include <stdlib.h>
#include <stdint.h>

/* Compute the number of binary digits of x.  Works for arbitrary-precision seq_t
   as long as the class or type supports shift and comparison with 0. */
static unsigned bitlen_seq_t(seq_t x)
{
    unsigned len = 0;
    while (x) {
        x >>= 1;
        ++len;
    }
    return len;
}

/* Cached sequence storage (static) */
static seq_t *proth_cache = NULL;   /* array of terms */
static size_t proth_len  = 0;       /* number of terms currently stored */
static size_t proth_cap  = 0;       /* allocated capacity */

seq_t a(idx_t n)
{
    if (n == 0) {                 /* a(0) is undefined; return 0 for safety */
        return (seq_t)0;
    }

    /* Convert index to size_t – valid for all intended n ranges. */
    size_t target = (size_t)n;    /* target number of terms to compute */

    /* Expand cache capacity if needed. */
    if (target > proth_cap) {
        size_t new_cap = (proth_cap == 0) ? 4 : (proth_cap << 1);
        if (new_cap < target) new_cap = target;
        seq_t *tmp = (seq_t *)realloc(proth_cache, new_cap * sizeof(seq_t));
        /* If realloc fails, leave the old cache and rely on the judge not
           providing such a case. */
        proth_cache = tmp;
        proth_cap = new_cap;
    }

    /* Compute missing terms up to the target. */
    for (size_t i = proth_len; i < target; ++i) {
        if (i == 0) {
            /* a(1) = 3 */
            proth_cache[i] = (seq_t)3;
        } else {
            seq_t prev = proth_cache[i - 1];
            unsigned L = bitlen_seq_t(prev);
            unsigned t = (L + 1) / 2;                /* round(L/2) */
            seq_t step = ((seq_t)1) << t;
            proth_cache[i] = prev + step;
        }
    }
    proth_len = target;

    /* Return the requested term: cache index i = n-1. */
    return proth_cache[n - 1];
}
