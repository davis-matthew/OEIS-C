/*
 * A234460  Imaginary part of the product of all the integer complex numbers
 *          in the square [1,1] to [n,n].
 * https://oeis.org/A234460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n even, and for n odd a(n) =
 *            Im(∏_{a=1}^{n}∏_{b=1}^{n}(a+bi))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if ((n & 1) == 0) /* even n -> product is real, imaginary part is zero */
        return (seq_t)0;

    seq_t real = 1;
    seq_t imag = 0;

    for (idx_t a = 1; a <= n; ++a) {
        seq_t a_seq = (seq_t)a;
        for (idx_t b = 1; b <= n; ++b) {
            seq_t b_seq = (seq_t)b;
            /* Multiply (real + imag*i) by (a + b*i) */
            seq_t new_real = real * a_seq - imag * b_seq;
            seq_t new_imag = real * b_seq + imag * a_seq;
            real = new_real;
            imag = new_imag;
        }
    }
    return imag;
}
