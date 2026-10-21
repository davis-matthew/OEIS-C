/*
 * A079645  Numbers j such that the integer part of the cube root of j divides
 *          j.
 * https://oeis.org/A079645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 2535, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Compute the n-th integer j such that floor(cuberoot(j)) divides j.
   The sequence is defined as the ordered list of all j≥1 satisfying that 
   property. We generate it by iterating over k = floor(cuberoot(j)) in
   increasing order and outputting all multiples k*m where m ranges from
   k² to k²+3k+3 (the multiples of k in the interval [k³, (k+1)³–1]).
   Complexity is O(n); for n up to 10⁴ this is well below the time limit.
*/

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t m_start = k * k;                /* = k² */
        idx_t m_end   = m_start + 3 * k + 3;   /* k² + 3k + 3 */
        for (idx_t m = m_start; m <= m_end; ++m) {
            ++count;
            if (count == n) {
                return (seq_t)k * (seq_t)m;
            }
        }
    }
}
