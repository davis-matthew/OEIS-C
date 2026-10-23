/*
 * A022057  Theta series of D_26 lattice.
 * https://oeis.org/A022057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 30, u128 n <= 1000,
 *            big any n
 * Helpers:   _floor_isqrt_u64, alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the coefficients of the theta series of the
   D_26 lattice:
   a(n) = number of integer vectors (x1,…,x26) with even sum and
          x1^2+…+x26^2 = 2n.
   The implementation uses dynamic arrays of seq_t allocated by the
   helper functions alloc_seq_array and free_seq_array.  It runs in
   O(n) time with moderate memory overhead. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Two times the norm bound.  All indices will be at most this. */
    idx_t maxSum = n * 2;
    size_t len = (size_t)maxSum + 1;

    /* Allocate four arrays for the current and next parity states. */
    seq_t *even_cur  = alloc_seq_array(len);
    seq_t *odd_cur   = alloc_seq_array(len);
    seq_t *even_next = alloc_seq_array(len);
    seq_t *odd_next  = alloc_seq_array(len);
    if (!even_cur || !odd_cur || !even_next || !odd_next)
        return 0;           /* allocation failure → return 0 */

    /* Initialise all entries to 0. */
    for (size_t i = 0; i < len; ++i) {
        even_cur[i]  = 0;
        odd_cur[i]   = 0;
        even_next[i] = 0;
        odd_next[i]  = 0;
    }

    even_cur[0] = 1;   /* zero vector has even parity and norm 0 */

    /* Maximal coordinate needed for the square bound. */
    idx_t maxM = _floor_isqrt_u64(maxSum);

    /* Process the 26 coordinates one by one. */
    for (idx_t dim = 0; dim < 26; ++dim) {
        /* Reset the next arrays. */
        for (size_t i = 0; i < len; ++i) {
            even_next[i] = 0;
            odd_next[i]  = 0;
        }

        for (size_t s = 0; s < len; ++s) {
            seq_t w_even = even_cur[s];
            seq_t w_odd  = odd_cur[s];
            if (w_even == 0 && w_odd == 0)
                continue;

            for (idx_t a = 0; a <= maxM; ++a) {
                size_t a_sq  = (size_t)a * (size_t)a;
                size_t s_new = s + a_sq;
                if (s_new >= len)
                    break;

                unsigned int parity = (unsigned int)(a & 1);
                seq_t cnt = (a == 0) ? 1 : 2;  /* multiplicity of ±a */

                if (w_even != 0) {
                    if (parity == 0)
                        even_next[s_new] = even_next[s_new] + w_even * cnt;
                    else
                        odd_next[s_new]  = odd_next[s_new]  + w_even * cnt;
                }

                if (w_odd != 0) {
                    unsigned int npar = parity ^ 1;
                    if (npar == 0)
                        even_next[s_new] = even_next[s_new] + w_odd * cnt;
                    else
                        odd_next[s_new]  = odd_next[s_new]  + w_odd * cnt;
                }
            }
        }

        /* Advance to the next dimension. */
        seq_t *tmp = even_cur; even_cur = even_next; even_next = tmp;
        tmp = odd_cur;          odd_cur  = odd_next;  odd_next  = tmp;
    }

    /* The number of vectors with squared norm 2n is in the
       even parity array at index maxSum. */
    seq_t result = even_cur[(size_t)maxSum];

    /* Free all temporary arrays. */
    free_seq_array(even_cur);
    free_seq_array(odd_cur);
    free_seq_array(even_next);
    free_seq_array(odd_next);

    return result;
}
