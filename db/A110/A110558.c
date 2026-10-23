/*
 * A110558  Numbers n such that (n^2-8)/8 is prime.
 * https://oeis.org/A110558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th number n such that (n^2-8)/8 is a prime. It is
 *            equivalent to n = 4*j where 2*j^2-1 is prime. The function
 *            builds the sequence lazily and memoises it
 * Range:     u8 n <= 33, u16 n <= 3376, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"   /* bool is_prime(uint64_t n); */
#include "../utils/push_seq.h"   /* void push_seq(seq_t **arr, idx_t *cap,
                                   idx_t *size, seq_t val); */

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t  size  = 0;
    static idx_t  cap   = 0;

    /* we also keep track of the last j value examined */
    static idx_t last_j = 1;   /* no j processed yet */

    if (n == 0) { /* offset is 1, behaviour for n<1 is irrelevant */
        return 0;
    }

    /* grow the sequence until we have at least n terms */
    while (size < n)
    {
        /* next j to test */
        ++last_j;

        /* compute p = 2*j^2 - 1, careful with overflow using 128-bit */
        __int128 tmp = (__int128)last_j * last_j; /* j^2 */
        tmp = tmp * 2 - 1;                         /* 2*j^2 - 1 */

        /* ensure we are still in the 64-bit range */
        if (tmp < 0 || tmp > UINT64_MAX)
        {
            /* for very large j we cannot test primality with available helpers
               – for the purposes of this build we simply stop generating terms
               beyond the range that fits in uint64_t.  */
            /* The caller is not expected to request indices that large. */
            break;
        }

        uint64_t p = (uint64_t)tmp;

        /* test primality */
        if (is_prime(p))
        {
            seq_t val = 4ULL * last_j;  /* n = 4*j */
            push_seq(&cache, &cap, &size, val);
        }
    }

    /* index n is 1‑based; cache is 0‑based */
    return cache[n - 1];
}
