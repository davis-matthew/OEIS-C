/*
 * A055881  a(n) = largest m such that m! divides n.
 * https://oeis.org/A055881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest m such that m! divides n
 * Range:     u8 n <= 9504, u16 n <= 9504, u32 n <= 9504, u64 n <= 9504,
 *            u128 n <= 9504, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the largest integer m (m ≥ 1) such that m! divides n.
   n is supplied as idx_t (≥ 1 and typically a 64‑bit unsigned integer).
   The result is returned in type seq_t (an unsigned integer type
   chosen by the build).  The algorithm iteratively builds factorials
   while they remain ≤ n; division checks are performed with idx_t
   arithmetic only, so no overflow occurs for n ≤ UINT64_MAX. */
seq_t a(idx_t n)
{
    idx_t factorial = 1;   /* current m! (as idx_t to avoid overflow) */
    seq_t  result   = 0;   /* store the largest m found so far */
    idx_t  m        = 1;   /* m starts at 1 because 1! divides all n */

    while (1) {
        /* If multiplying by m would exceed n or overflow, stop. */
        if (factorial > n / m) break;

        factorial *= m;                /* now factorial = m! */
        if (n % factorial == 0) {      /* m! divides n */
            result = (seq_t)m;
        } else {                        /* larger factorials cannot divide n */
            break;
        }
        ++m;
    }
    return result;
}
