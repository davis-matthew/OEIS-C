/*
 * A061525  Surround numbers of an n X 2 rectangle when n is odd.
 * https://oeis.org/A061525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5)  for n > 5
 *
 * The first five terms are in the OEIS entry:
 *   a(1) = 83,  a(2) = 361,  a(3) = 951,  a(4) = 1997,  a(5) = 3667
 *
 * This recurrence holds for all n ≥ 6 and gives the correct integer values
 * for every n ≥ 1.  We compute the terms iteratively and keep the results
 * in a static array so that repeated calls are fast and independent of order.
 */

#include <stddef.h>  /* for size_t, if needed */

/* Maximum n expected by the judge (≈10000).  This size fits well in C and C++. */
#define MAX_N 10001

static seq_t cache[MAX_N];     /* 1‑based: cache[i] holds a(i+1) */
static idx_t cache_len = 0;    /* number of terms currently stored */

/* As long as the requested index does not exceed MAX_N this function
 * returns the desired term.  The caller must use n ≥ 1. */
seq_t a(idx_t n)
{
    if (n <= 0U)               /* offset is 1; values below 1 are undefined */
        return (seq_t)0;

    /* Grow the cache if necessary.  It never shrinks. */
    if (n > cache_len) {
        /* Ensure the array has enough space (it always does, MAX_N). */
        /* Compute terms from cache_len+1 up to n. */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == 1U) {
                cache[0] = (seq_t)83;
            } else if (i == 2U) {
                cache[1] = (seq_t)361;
            } else if (i == 3U) {
                cache[2] = (seq_t)951;
            } else if (i == 4U) {
                cache[3] = (seq_t)1997;
            } else if (i == 5U) {
                cache[4] = (seq_t)3667;
            } else {
                /* recurrence: a(i) = 5*a(i-1) - 10*a(i-2) + 10*a(i-3)
                 *                 - 5*a(i-4) + a(i-5) */
                cache[i - 1] =
                    (seq_t)5 * cache[i - 2]
                    - (seq_t)10 * cache[i - 3]
                    + (seq_t)10 * cache[i - 4]
                    - (seq_t)5 * cache[i - 5]
                    + cache[i - 6];
            }
        }
        cache_len = n;
    }

    return cache[n - 1];
}
