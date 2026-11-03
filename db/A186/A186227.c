/*
 * A186227  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f and g are the triangular numbers and
 *          heptagonal numbers. Complement of A186228.
 * https://oeis.org/A186227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + number of heptagonal numbers < triangular(n)
 * Range:     u8 n <= 21, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangular numbers:   T_k = k(k+1)/2 */
/* Heptagonal numbers:   H_k = k(5k-3)/2 */
/* Ties are eliminated by ranking the triangular number first.  */

#include <stdint.h>
#include <stdbool.h>

/* return count of heptagonal numbers strictly less than X */
static seq_t hept_less_than(seq_t X)
{
    seq_t count = 0;
    seq_t k = 1;
    while (1) {
        /* compute H_k = k(5k-3)/2 */
        seq_t val = k * (5 * k - 3) / 2;
        if (val >= X) break;
        ++count;
        ++k;
    }
    return count;
}

/* compute triangular number T_k */
static inline seq_t triangular(idx_t k)
{
    return (seq_t)k * ((seq_t)k + 1) / 2;
}

seq_t a(idx_t n)
{
    seq_t t = triangular(n);
    seq_t hcnt = hept_less_than(t);
    return n + hcnt;
}
