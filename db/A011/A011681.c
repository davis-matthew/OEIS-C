/*
 * A011681  A binary m-sequence: expansion of reciprocal of
 *          x^7+x^6+x^5+x^3+x^2+x+1.
 * https://oeis.org/A011681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=1, a(n) =
 *            a(n-1) XOR a(n-2) XOR a(n-3) XOR a(n-5) XOR a(n-6) XOR a(n-7)
 *            for n≥7
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* period is 127, derived from the primitive polynomial
       x^7 + x^6 + x^5 + x^3 + x^2 + x + 1 over GF(2). */
    static seq_t seq[127];
    static bool init = false;
    if (!init) {
        /* initial state derived from the first seven coefficients
           of the reciprocal series in GF(2). */
        seq[0] = 0; seq[1] = 0; seq[2] = 0;
        seq[3] = 0; seq[4] = 0; seq[5] = 0;
        seq[6] = 1;
        for (idx_t i = 7; i < 127; ++i) {
            seq[i] = seq[i-1] ^ seq[i-2] ^ seq[i-3]
                           ^ seq[i-5] ^ seq[i-6] ^ seq[i-7];
        }
        init = true;
    }
    /* wrap index by the period */
    idx_t k = n % (idx_t)127;
    return seq[k];
}
