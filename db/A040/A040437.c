/*
 * A040437  Continued fraction for sqrt(459).
 * https://oeis.org/A040437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of continued fraction for sqrt(459). 
   a(0)=21, and for n>=1 the sequence repeats the period 10 
   block: 2,2,1,4,21,4,1,2,2,42. */
#include <stdbool.h>
#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)21;
    }
    static seq_t block[10];
    static bool init = false;
    if (!init) {
        uint64_t m = 0;
        uint64_t d = 1;
        uint64_t a0 = 21;
        uint64_t a = a0;
        for (int i = 0; i < 10; ++i) {
            uint64_t m_next = d * a - m;
            uint64_t d_next = (459 - m_next * m_next) / d;
            uint64_t a_next = (a0 + m_next) / d_next;
            block[i] = (seq_t)a_next;
            m = m_next;
            d = d_next;
            a = a_next;
        }
        init = true;
    }
    return block[(n - 1) % 10];
}
