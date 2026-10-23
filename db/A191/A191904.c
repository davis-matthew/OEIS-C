/*
 * A191904  Square array read by antidiagonals up: T(n,k) = 1-k if k divides
 *          n, else 1.
 * https://oeis.org/A191904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   not stated
 * Range:     i8 n <= 120, i16 n <= 120, i32 n <= 120, i64 n <= 120,
 *            i128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computation of OEIS sequence A191904.
 * The square array is defined by 
 *     T(n,k) = 1 - k   if k divides n
 *            = 1        otherwise
 * The sequence a(n) is obtained by reading the array
 * along antidiagonals “up”: the sum n+k is fixed; for a
 * given sum we list the pairs (n,k) with n decreasing from
 * the sum-1 down to 1. 
 * Example: a(1)=T(1,1)=0, a(2)=T(2,1)=0, a(3)=T(1,2)=1, ...
 *
 * The function returns the nth term (1‑based offset).
 * Complexity is O(√n) by scanning for the diagonal length.
 */

#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build.
 * They may be signed or unsigned 64/128 bit integers or
 * arbitrary precision integer types.
 */

seq_t a(idx_t n)
{
    /* sequence is defined for n >= 1; if n==0 return 0 as fallback */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Find the diagonal s such that (s-1)*s/2 >= n.
     * s is the sum n_k + k_k of the pair belonging to a(n).
     */
    idx_t s = 1;          /* current diagonal sum (will become >=2) */
    idx_t total = 0;      /* number of terms up to current diagonal */

    /* Incrementally increase the diagonal until the cumulative count
     * reaches or exceeds n.  Number of terms in diagonal s is (s-1). */
    while (total < n) {
        ++s;
        total += s - 1;
    }

    /* total now equals (s-1)*s/2, which is the number of terms up to
     * and including diagonal s.  The previous diagonal cumulative
     * count is (s-2)*(s-1)/2. */
    idx_t prev_total = total - (s - 1);

    /* Position inside the current diagonal (1‑based). */
    idx_t o = n - prev_total;   /* 1 <= o <= s-1 */

    /* Recover n_k and k_k for this position.  In the diagonal the
     * first element has n = s-1 and k = 1; the last has n = 1 and k = s-1.
     * Mapping: n_k = s - o,   k_k = o. */
    idx_t n_k = s - o;
    idx_t k_k = o;

    /* Compute T(n_k, k_k).  Use seq_t for the arithmetic to keep
     * the result in the expected precision. */
    if (n_k % k_k == 0) {
        return (seq_t)1 - (seq_t)k_k;
    } else {
        return (seq_t)1;
    }
}
