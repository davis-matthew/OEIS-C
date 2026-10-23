/*
 * A024953  a(n) = Sum_{i=1..floor((n+2)/4)} a(2i-1)*a(n-2i+1), with
 *          a(1)=a(2)=1 and a(3)=6.
 * https://oeis.org/A024953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..⌊(n+2)/4⌋} a(2i‑1) * a(n-2i+1) with initial
 *            values a(1)=1, a(2)=1, a(3)=6
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The sequence is computed iteratively with memoisation.  The cache
   size is chosen to handle n up to about 50000 comfortably; this
   exceeds the required 10000 limit.  For smaller n the array remains
   unused.  The cache is statically allocated to avoid the
   complications of dynamic allocation with arbitrary‑precision
   types. */
#define MAX_CACHE 50000
static seq_t cache[MAX_CACHE + 1];   /* zero‑initialised */
static int base_set = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined but harmless */

    /* initialise the base values once */
    if (!base_set)
    {
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 6;
        base_set = 1;
    }

    if (n <= 3)           /* return the prescribed base values */
        return (n == 1 || n == 2) ? 1 : 6;

    /* compute the sequence values up to n if they have not yet been
       computed.  Each value is stored once and reused afterwards. */
    for (idx_t k = 4; k <= n; ++k)
    {
        if (cache[k] != 0) continue;   /* already computed */

        idx_t bound = (k + 2) / 4;
        seq_t sum = 0;
        for (idx_t i = 1; i <= bound; ++i)
        {
            idx_t left  = 2 * i - 1;           /* guaranteed < k */
            idx_t right = k - 2 * i + 1;       /* guaranteed ≥ 1 */
            sum += cache[left] * cache[right];
        }
        cache[k] = sum;
    }

    return cache[n];
}
