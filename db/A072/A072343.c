/*
 * A072343  a(n) = 2^(n-1) * Sum_{k=0..n} (n+k)! / k!^2.
 * https://oeis.org/A072343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A072343
 *  Formula: a(n) = 2^(n-1) * Σ_{k=0..n} (n+k)! / k!^2
 *
 *  The implementation caches factorials in a dynamic array to keep an
 *  amortised O(1) time per term even when calls are unordered.  The
 *  summation uses a recurrence that requires only two temporary
 *  values, avoiding storage of all intermediate terms.  All operations
 *  are performed with the user‑supplied integer type `seq_t`; the
 *  code works unchanged for 8/16/32/64‑bit builds and for the
 *  arbitrary‑precision C++ build.
 *
 *  The code is pure C99/C11 and also compiles as C++17.  No external
 *  headers beyond <stdlib.h> are used.
 */

#include <stdlib.h>

/* ------------------------------------------------------------ */
/*  a(idx_t n)  – compute the n-th term of A072343, offset 1   */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* n must be >=1 because the OEIS offset is 1 */
    if (n == 0)
        return 0;                    /* never reached, but keeps the compiler quiet */

    /* ----------  cache factorials --------------------------------- */
    /* static variables are shared across all calls to this function */
    static seq_t *fact_cache = NULL;   /* array of factorials: fact_cache[i] = i!    */
    static idx_t  cache_size = 0;      /* number of entries currently available */

    /* Grow the cache if needed */
    if ((idx_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();      /* allocation failed */
        fact_cache = new_cache;

        /* compute the new factorials */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0)
                fact_cache[0] = (seq_t)1;                /* 0! = 1 */
            else
                fact_cache[i] = fact_cache[i-1] * (seq_t)i; /* i! = (i-1)! * i */
        }
        cache_size = new_size;
    }

    /* ----------  compute the sum Σ (n+k)! / k!^2  ------------------- */
    seq_t term   = fact_cache[n];    /* T(0) = (n+0)! / 0!^2 = n!   */
    seq_t sum    = (seq_t)0;          /* running sum                       */

    for (idx_t k = 0; k <= n; ++k) {
        sum += term;                      /* add current term */

        /* prepare next term for k+1 (if k < n) */
        if (k < n) {
            idx_t kk   = k + 1;                         /* k+1            */
            seq_t num  = (seq_t)(n + k + 1);            /* n+k+1          */
            seq_t den  = (seq_t)(kk * kk);              /* (k+1)^2 */
            term = (term * num) / den;                  /* exact division*/
        }
    }

    /* ----------  multiply by 2^(n-1)   --------------------------------- */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 1; i <= n - 1; ++i)
        pow2 *= (seq_t)2;                /* safe multiplication */

    /* ----------  final result -------------------------------------- */
    return sum * pow2;
}
