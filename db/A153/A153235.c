/*
 * A153235  Numbers n such that 8*n+7 is not prime.
 * https://oeis.org/A153235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n such that 8*n+7 is composite (i.e. not prime)
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation lazily builds the sequence and caches the
   results.  The primality test is a simple trial division that works
   for the values encountered (8*n+7 is always < 8*10⁴+7 for the
   required input range).  It uses only the operations allowed on
   seq_t. */

#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of the function used to compute the sequence. */
static bool is_composite(seq_t x);

/*
 * Returns the nth term of the sequence A153235.
 *
 * a(1) = 1, a(2) = 4, a(3) = 6, ...
 * The function is thread‑safe only for single‑threaded use; it uses
 * static storage to memoise previously computed terms.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset starts at 1; behavior for n==0 is arbitrary */

    /* Cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of entries stored */
    static idx_t next_k = 1;      /* next integer k to test */
    static size_t cache_cap = 0;  /* allocated capacity of cache */

    /* Extend the cache until it contains at least n terms */
    while (cache_len < n) {
        /* Compute 8*k + 7 as seq_t value */
        seq_t val = ((seq_t)8 * (seq_t)next_k) + (seq_t)7;

        if (is_composite(val)) {
            /* Grow the cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();   /* unrecoverable allocation error */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }

        ++next_k;
    }

    return cache[n - 1];
}

/* Simple trial division primality test – returns true if x is composite. */
static bool is_composite(seq_t x)
{
    /* 8*n+7 is always odd and ≥ 7, so we only need to test odd divisors. */
    if (x <= 3)
        return false;

    seq_t d = 3;
    while (1) {
        seq_t d2 = d * d;
        if (d2 > x)
            break;
        if (x % d == 0)
            return true;
        d += 2;
    }
    return false;
}
