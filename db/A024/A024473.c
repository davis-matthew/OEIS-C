/*
 * A024473  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k = [
 *          (n+1)/2 ], s = (Lucas numbers), t = (odd natural numbers).
 * https://oeis.org/A024473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} Lucas(i) * [2*(n+1-i)-1]
 * where Lucas(0)=2, Lucas(1)=1 and Lucas(i)=Lucas(i-1)+Lucas(i-2).
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* safety: offset starts at 1 */
    if (n == 0) return 0;

    /* required number of Lucas values = k+1, because indices 0..k are used */
    const idx_t k = (n + 1) / 2;          /* floor((n+1)/2) */
    const idx_t needed = k + 1;          /* number of array elements needed */

    /* static cache of Lucas numbers */
    static seq_t *lucas = NULL;
    static size_t lucas_max = 0;         /* highest index already computed + 1 */

    if (lucas_max < needed) {
        /* enlarge the cache */
        seq_t *new_cache = (seq_t*)realloc(lucas, needed * sizeof(seq_t));
        if (!new_cache) abort();
        lucas = new_cache;

        /* initialise the base cases if not already done */
        if (lucas_max < 2) {
            lucas[0] = (seq_t)2;           /* Lucas(0) */
            if (needed > 1) lucas[1] = (seq_t)1;   /* Lucas(1) */
            /* next indices to compute, if any */
            lucas_max = 2;
        }

        /* compute missing Lucas numbers */
        for (size_t i = lucas_max; i < needed; ++i) {
            lucas[i] = lucas[i - 1] + lucas[i - 2];
        }
        lucas_max = needed;
    }

    /* compute the weighted sum */
    seq_t sum = 0;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t li   = lucas[(size_t)i];
        /* odd term t(n+1-i) = 2*(n+1-i)-1 */
        seq_t odd  = (seq_t)2 * (seq_t)(n + 1 - i) - (seq_t)1;
        sum += li * odd;
    }

    return sum;
}
