/*
 * A046736  Number of ways to place non-intersecting diagonals in convex n-gon
 *          so as to create no triangles.
 * https://oeis.org/A046736
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   A000 ... * The sequence satisfies the cubic generating‑function
 *            equation * * A(x) – A(x)² – A(x)³ = x·(1 – A(x)) * * Let a₁ =
 *            a(2) = 1, a₂ = a(3) = 0, a₃ = a(4) = 1, a₄ = a(5) = 1. * For m ≥
 *            2 the coefficients of the power series expansion obey * * a_m =
 *            Σ_{i=1}^{m-1} a_i·a_{m-i} * + Σ_{i=1}^{m-2...
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 25, u64 n <= 43, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>        /* for size_t */

/* The maximum n we support.  Sequence values grow very fast, but the required
   range is only about 10 000.  We use a slightly larger buffer to be safe. */
#define MAX_N 20000

#if defined(__cplusplus)
#include <type_traits>
using conv_t = typename std::conditional<
                 std::is_integral<seq_t>::value &&
                 sizeof(seq_t) <= 8,
                 unsigned __int128,
                 seq_t>::type;
#else
typedef unsigned __int128 conv_t;
#endif

static seq_t a_arr[MAX_N]  = {0};   /* a_k  for k = 1 .. MAX_N-1  (k = n-1)      */
static conv_t conv_arr[MAX_N] = {0};/* C_k = Σ_{i=1}^{k-1} a_i a_{k-i}              */
static idx_t max_k = 0;             /* largest k for which a_arr[k] is known     */
static int  initialized = 0;        /* one‑time initialisation flag              */

seq_t a(idx_t n)
{
    if (n < 2)                  /* outside the defined region             */
        return 0;

    /* n-th term needs a value for k = n-1                             */
    idx_t target_k = n - 1;
    if (target_k >= MAX_N)
        return 0;               /* out of range – the build may not request this */

    /* initialise data on first call                                  */
    if (!initialized) {
        a_arr[1] = (seq_t)1;    /* a(2) = 1                                    */
        /* a_arr[2] (corresponds to a(3)) remains 0                 */
        /* a_arr[3] and a_arr[4] will be produced by the recurrence */
        max_k = 1;              /* we have computed up to k = 1                */
        initialized = 1;
    }

    /* extend the precomputed table until the requested k          */
    if (max_k < target_k) {
        for (idx_t k = max_k + 1; k <= target_k; ++k) {
            conv_t sum1 = 0;                       /* Σ a_i·a_{k-i}                        */
            for (idx_t i = 1; i < k; ++i)
                sum1 += (conv_t)a_arr[i] * (conv_t)a_arr[k - i];

            conv_arr[k] = sum1;                     /* store C_k                             */

            conv_t sum2 = 0;                       /* Σ a_i·C_{k-i}                        */
            for (idx_t i = 1; i < k; ++i)
                sum2 += (conv_t)a_arr[i] * conv_arr[k - i];

            conv_t tmp = sum1 + sum2 - (conv_t)a_arr[k - 1];
            a_arr[k] = (seq_t)tmp;                 /* cast back – exact for all sequence   */
        }
        max_k = target_k;
    }

    return a_arr[n - 1];
}
