/*
 * A066321  Binary representation of base-(i-1) expansion of n: replace i-1
 *          with 2 in base-(i-1) expansion of n.
 * https://oeis.org/A066321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Binary representation of base-(i-1) expansion of n
 *            (replace i-1 by 2 in the base-(i-1) expansion)
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Gaussian integer representation a + bi of current value.
       Start with the integer n as the real part. */
    int64_t a_real = (int64_t)n;
    int64_t b_imag = 0;
    seq_t result = 0;

    /* Maximum number of bits we may set in the result without overflow. */
    const size_t max_bits = sizeof(seq_t) * 8;

    size_t pos = 0;                     /* position of the current digit */
    while (a_real != 0 || b_imag != 0) {
        /* Choose the remainder r ∈ {0,1} such that (a_real – r + i·b_imag)
           is divisible by (i‑1).  The parity condition is
               r ≡ a_real – b_imag (mod 2). */
        int r = (int)((a_real - b_imag) & 1LL);

        if (r && pos < max_bits) {
            result |= ((seq_t)1 << pos);
        }

        /* Compute the next Gaussian integer after dividing by i‑1:
           q = (a + bi – r) / (i‑1) = ((-a + r + b)/2) + i·((-a + r – b)/2). */
        int64_t new_a = (-a_real + r + b_imag) >> 1;
        int64_t new_b = (-a_real + r - b_imag) >> 1;

        a_real = new_a;
        b_imag = new_b;
        ++pos;
    }
    return result;
}
