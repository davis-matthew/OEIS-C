/*
 * A043544  Numbers k such that s(k)<s(k-1), where s(k)=sum of all digits in
 *          all base b representations of k, for 2<=b<=k.
 * https://oeis.org/A043544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   sum_digits_in_base, sum_of_digits_all_bases
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Compute A043544: numbers k such that s(k) < s(k-1), where 
   s(k) = sum of all digits of k in all bases b, 2 ≤ b ≤ k.
   This implementation evaluates s(k) directly by iterating the bases
   and searches for the n‑th term.  It requires only integer
   arithmetic and works for any seq_t / idx_t types that support the
   usual operators.  No external helper functions are used.
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sum_digits_in_base.h"
#include "../utils/sum_of_digits_all_bases.h"

/* Forward declaration of the external function */
seq_t a(idx_t n);

/* Main sequence function */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1: a(1) is the first term (12). */
    if (n == 0) return (seq_t)0;        /* undefined, but safe fallback */

    /* Count of terms found so far */
    idx_t found = 0;

    /* Compute s(2) once as the previous value for k = 3,4,... */
    uint64_t k = 2;
    seq_t prev_s = sum_of_digits_all_bases(k);

    for (k = 3;; ++k)
    {
        seq_t cur_s = sum_of_digits_all_bases(k);
        if (cur_s < prev_s)
        {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
        prev_s = cur_s;
    }
}
