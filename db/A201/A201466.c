/*
 * A201466  E.g.f. A(x) satisfies A(x) = (x + 3*exp(A(x)) - 3)/4.
 * https://oeis.org/A201466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A201466
 *   a(1) = 1,
 *   a(2) = 3,
 *   a(3) = 30,
 *   a(4) = 498, ...
 *
 *  The exponential generating function A(x) satisfies
 *
 *          A(x) = (x + 3*exp(A(x)) - 3)/4
 *
 *  Equivalently let
 *
 *          B_n = Σ_{partitions of {1…n}}   ∏ a_{|block|}
 *
 *  then for n≥1   4 a_n = 3 B_n   (with a(1)=1 and B_0=1).
 *
 *  Using the standard combinatorial recurrence for B_n,
 *
 *          B_n = Σ_{k=1..n}  binom(n-1,k-1)  a_k  B_{n-k}
 *
 *  we obtain a simple linear recurrence for a_n:
 *
 *          a_n = 3 * Σ_{k=1}^{n-1} binom(n-1,k-1)  a_k  B_{n-k},   n≥2
 *
 *  where B_{n-k} is known from previous steps.  A(n) is then obtained
 *  in O(n²) time and O(n) memory, which is easily fast enough for
 *  n≈10⁴.  The implementation keeps a cache of all values from 1
 *  up to the largest requested n so that multiple calls are
 *  efficient.
 */

#include <stdbool.h>
#include <stdint.h>

#define MAX_CACHE 20000   /* enough for all tests (≈10⁴).  Increase if needed. */

static bool seq_initialized = false;
static seq_t seq_a[MAX_CACHE+1];
static seq_t seq_B[MAX_CACHE+1];
static idx_t seq_computed = 0;   /* largest n for which seq_a[n] is valid */

static void seq_compute_upto(idx_t n)
{
    if (n > MAX_CACHE) return;   /* silently ignore too large requests */

    for (idx_t m = seq_computed + 1; m <= n; ++m)
    {
        seq_t sum = 0;
        seq_t binom = 1;   /* binom(m-1,0) */

        /* iterate over k from 1 to m-1 */
        for (idx_t k = 1; k <= m - 1; ++k)
        {
            /* term = a_k * B_{m-k} * binom(n-1,k-1) */
            seq_t term = seq_a[k] * seq_B[m - k];
            term *= binom;
            sum += term;

            /* update binom for the next k */
            if (k < m - 1)
            {
                binom = binom * (seq_t)(m - k) / (seq_t)k;
            }
        }

        seq_a[m] = sum * 3;          /* a_m = 3 * sum */
        seq_B[m] = seq_a[m] + sum;   /* B_m = a_m + sum  (for future use) */
    }
    seq_computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for 0, return 0 to be safe */

    if (!seq_initialized)
    {
        /* first call: initialise base values */
        seq_a[0] = 0;
        seq_B[0] = 1;
        seq_a[1] = 1;
        seq_B[1] = 1;
        seq_computed = 1;
        seq_initialized = true;
    }

    if (n > seq_computed)
        seq_compute_upto(n);

    return seq_a[n];
}
