/*
 * A206332  Complement of A092754.
 * https://oeis.org/A206332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + 2^(floor(log2(n)) + 1) - 1
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sequence implemented by computing the next power of two after n */

/*  The function returns the nth term of OEIS A206332.  The index n is
 *  1‑based (a(1) = 2).  The implementation runs in O(log n), which
 *  easily satisfies the required time limits for n up to 10 000.  It
 *  uses only integer arithmetic and no external utilities.  It works
 *  for any unsigned seq_t and idx_t provided by the build system.  */

#include <stdint.h>
#include <stdbool.h>

/* helper: floor(log2(x)) for x > 0, using repeated shifts. */
static inline idx_t floor_log2_idx(idx_t x)
{
    idx_t r = 0;
    while (x > 1) {
        x >>= 1;
        ++r;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) {          /* undefined for 0, but return 0 for safety */
        return 0;
    }

    /* Compute k = floor(log2(n)) + 1, i.e. the exponent of the
     * smallest power of two strictly greater than n. */
    idx_t k = floor_log2_idx(n) + 1;
    seq_t pow = (seq_t)1 << k;          /* 2^k  */
    return (seq_t)n + pow - 1;
}
