/*
 * A057899  Smallest positive integer such that n=a(n)^k-k with k a positive
 *          integer.
 * https://oeis.org/A057899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*

   Recurrence : none.  
   The function searches for the smallest integer m such that  

            n = m^k – k        (k ≥ 1)

   The trivial solution k = 1 gives m = n + 1, so the answer is at most
   n + 1.  The algorithm tries all k ≥ 2 for which 2^k ≤ n + k,
   computes the integer k‑th root of n + k and checks whether it
   satisfies the equation.

   Complexity: O(log n * log n) per call, easily fast enough for
   the test ranges (n up to ~10⁴ or more).

*/

#include <stdint.h>

/* multiply base ^ exp but stop and return a value > limit as soon as the
   product exceeds limit; this avoids unnecessary large intermediate
   results.  seq_t, idx_t stays the same. */
static seq_t pow_limit(seq_t base, idx_t exp, seq_t limit)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
    {
        res *= base;
        if (res > limit)
            return limit + 1;     /* special value > limit */
    }
    return res;
}

/* integer k‑th root floor of value val.  Works for any seq_t that
   supports +, / and >. */
static idx_t kth_root_floor(seq_t val, idx_t k)
{
    seq_t low = 1, high = val;
    while (low < high)
    {
        seq_t mid = (low + high + 1) / 2;
        seq_t prod = pow_limit(mid, k, val);
        if (prod <= val)
            low = mid;
        else
            high = mid - 1;
    }
    return (idx_t)low;   /* low fits in idx_t because low ≤ val ≤ n+1 ≤ idx_t max */
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* trivial solution: m = n + 1 */
    seq_t best = (seq_t)n + 1;

    /* try all k ≥ 2.  For k large no m≥2 can satisfy 2^k ≤ n + k. */
    for (idx_t k = 2; k < 64; ++k)
    {
        seq_t val = (seq_t)n + k;

        /* fast exit: if even the smallest candidate m=2 is too large */
        if (pow_limit((seq_t)2, k, val) > val)
            break;

        idx_t r = kth_root_floor(val, k);
        if (pow_limit((seq_t)r, k, val) == val)
        {
            seq_t m_candidate = (seq_t)r;
            if (m_candidate < best)
                best = m_candidate;
        }
    }

    return best;
}
