/*
 * A008964  Number of elements of order <= 2 in group of n X n upper
 *          triangular matrices over GF(2).
 * https://oeis.org/A008964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{r=0..floor(n/2)} f(n,r) where f(1,0) = 1, f(n,0) =
 *            1, f(n,r) = 2^r * f(n-1,r) + (2^(n-r)-2^(r-1)) * f(n-1,r-1) (r
 *            >= 1) This definition counts the upper triangular matrices over
 *            GF(2) with (I+B)^2 = I, i.e., elements of order ≤ 2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                         /* for size_t */
#include "../utils/alloc_seq_array.h"       /* for alloc_seq_array */
#include "../utils/free_seq_array.h"        /* for free_seq_array */

seq_t a(idx_t n)
{
    if (n == 0)      /* offset is 1, but we return 0 for n=0 */
        return (seq_t)0;

    if (n == 1)
        return (seq_t)1;   /* D: a(1) = 1 */

    /* ---------- pre‑compute powers of two up to n ---------- */
    seq_t *pow2 = alloc_seq_array((size_t)(n + 1));
    if (!pow2)
        return (seq_t)0;      /* out of memory */

    pow2[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow2[i] = pow2[i - 1] * (seq_t)2;

    /* ---------- DP arrays for f(n,r) ---------------------- */
    size_t max_r = n / 2 + 1;                 /* maximum number of r indices */
    seq_t *fprev = alloc_seq_array(max_r);    /* f for n-1   */
    seq_t *fcur  = alloc_seq_array(max_r);    /* f for n   */
    if (!fprev || !fcur) {
        free_seq_array(pow2);
        if (fprev) free_seq_array(fprev);
        if (fcur)  free_seq_array(fcur);
        return (seq_t)0;
    }

    /* initialize for n = 1 */
    fprev[0] = (seq_t)1;
    size_t prev_size = 1;                     /* floor( (1-1)/2 ) + 1 */

    /* ---------- iterate n = 2 .. n ------------------------ */
    for (idx_t cur_n = 2; cur_n <= n; ++cur_n) {
        size_t cur_size = cur_n / 2 + 1;      /* number of r values for current n */

        fcur[0] = (seq_t)1;                  /* f(cur_n,0) = 1 */

        for (idx_t r = 1; r < cur_size; ++r) {
            seq_t term1 = 0;
            if (r < prev_size)
                term1 = pow2[r] * fprev[r];

            seq_t diff = pow2[cur_n - r] - pow2[r - 1];
            seq_t term2 = diff * fprev[r - 1];

            fcur[r] = term1 + term2;
        }

        /* If this is the desired n, compute and return the sum */
        if (cur_n == n) {
            seq_t total = fcur[0];
            for (idx_t r = 1; r < cur_size; ++r)
                total += fcur[r];

            free_seq_array(pow2);
            free_seq_array(fprev);
            free_seq_array(fcur);
            return total;
        }

        /* Prepare for next iteration: fprev <- fcur, fcur <- old fprev */
        seq_t *tmp = fprev;
        fprev = fcur;
        fcur  = tmp;
        prev_size = cur_size;
    }

    /* Should never reach here for n >= 1 */
    free_seq_array(pow2);
    free_seq_array(fprev);
    free_seq_array(fcur);
    return (seq_t)0;
}
