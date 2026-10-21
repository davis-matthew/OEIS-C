/*
 * A096553  Consecutive states of the linear congruential pseudo-random number
 *          generator used in function rand() in the Standard C library (VAX
 *          C) when started at 1.
 * https://oeis.org/A096553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1103515245 * a(n‑1) + 12345) mod 2^31, a(1) = 1 (VAX C
 *            rand() linear congruential generator)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* cache[i] holds a(i) for i >= 1. cache[0] is unused.  The cache is
   grown on demand and keeps values in arbitrary order of calls.
   For builds where seq_t is a simple integer type this works
   directly.  For builds where seq_t is a class the caller must provide
   an allocator that performs placement new;  cast allocation is used
   here because the usual allocator utilities are not available in
   plain C. */

/* static variables -------------------------------------------------------- */
static seq_t *cache = NULL;     /* Allocated cache array                */
static idx_t cache_len = 0;     /* Highest index stored (0 if empty)   */

/* public function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                /* the sequence is defined only for n ≥ 1 */
        return 0;

    /* Ensure the cache is large enough for index n                     */
    if (cache_len < n) {
        /* Grow cache to hold indices 0 .. n inclusive                  */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) {
            abort();               /* allocation failure                    */
        }
        cache = tmp;

        /* Initialise new entries progressively to avoid taking
           memory for later indices that may never be needed.          */
        if (cache_len == 0) {
            /* First term explicit */
            cache[1] = (seq_t)1;
            cache_len = 1;
        }

        /* Compute missing terms up to n                               */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* Operate in 64‑bit arithmetic to avoid overflow before
               the final modulus.                                       */
            unsigned long long prev = (unsigned long long)cache[i - 1];
            unsigned long long val  =
                (1103515245ULL * prev + 12345ULL) & 0x7FFFFFFFULL; /* 2^31‑1 */
            cache[i] = (seq_t)val;
        }
        cache_len = n;
    }

    return cache[n];
}
