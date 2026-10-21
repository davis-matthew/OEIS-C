/*
 * A070282  a(1) = 1; a(2) = 2; a(n) = [sum of digits of a(n-1)]^[sum of
 *          digits of a(n-2)] for n > 2.
 * https://oeis.org/A070282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 1; a(2) = 2; a(n) = (sumdigits a(n-1)) ^ (sumdigits
 *            a(n-2)) for n>2 Sequence A070282
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Undefined for n<1: return 0 */
    if (n == 0)
        return (seq_t)0;

    /* We store terms in a zero‑based static array.
       cache[0] = a(1), cache[1] = a(2), ...           */
    static seq_t cache[10001];          /* enough for n up to 10000 */
    static bool ready[10001] = {false};
    static idx_t current_max = 0;       /* last index that has been computed */

    /* Initialise the first two terms if necessary */
    if (!ready[0]) { cache[0] = (seq_t)1; ready[0] = true; }
    if (!ready[1]) { cache[1] = (seq_t)2; ready[1] = true; }
    if (current_max < 2) current_max = 2;   /* we have at least a(1) and a(2) */

    /* Return early for n==1 or n==2 */
    if (n == 1) return cache[0];
    if (n == 2) return cache[1];

    /* Compute missing terms up to n */
    for (idx_t idx = current_max; idx < n; ++idx) {
        seq_t sd_prev  = digit_sum(cache[idx-1]); /* sumdigits a(idx)   */
        seq_t sd_prev2 = digit_sum(cache[idx-2]); /* sumdigits a(idx-1) */
        cache[idx] = big_pow(sd_prev, sd_prev2);
        ready[idx] = true;
    }
    current_max = n;

    return cache[n-1];
}
