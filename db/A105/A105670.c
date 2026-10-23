/*
 * A105670  a(1)=1 then bracketing n by powers of 2 as f(t)=2^t for f(t) < n
 *          <= f(t+1), a(n) = f(t+1) - a(n-f(t)).
 * https://oeis.org/A105670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence *A105670*:
   a(1) = 1
   For n > 1, let t = floor(log2(n-1)) (i.e. the largest integer with 2^t < n)
   Then a(n) = 2^(t+1) - a(n - 2^t)   (all operations on seq_t)
   The sequence repeats every two terms: a(2k-1) = a(2k).  */

#include <stdint.h>

/* Recursive helper to compute a(n).  Recursion depth is O(log n). */
static seq_t a_rec(idx_t n)
{
    if (n <= 1)           /* base case: a(1) = 1 */
        return (seq_t)1;

    /* compute t = floor(log2(n-1)) */
    idx_t m = n - 1;
    idx_t t = 0;
    while (m > 1) {       /* loop while the next higher bit exists */
        m >>= 1;
        ++t;
    }

    /* 2^(t+1) as seq_t */
    seq_t pow = ((seq_t)1) << (t + 1);

    /* 2^t as idx_t for the offset */
    idx_t offset = ((idx_t)1) << t;

    return pow - a_rec(n - offset);   /* recurrence */
}

/* External function required by the problem statement. */
seq_t a(idx_t n)
{
    return a_rec(n);
}
