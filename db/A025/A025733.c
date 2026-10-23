/*
 * A025733  Index of 8^n within the sequence of the numbers of the form
 *          8^i*10^j.
 * https://oeis.org/A025733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1) + Σ_{k=0..n} ⌊k * log10(8)⌋ * (which equals the
 *            definition 1 + Σ ceil(k*c) with c = log10(8))
 * Range:     u8 n <= 22, u16 n <= 379, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define C_Q 1000000000000000ULL          // scaling factor for the rational approximation
#define C_R 903089986991943ULL          // floor(log10(8) * C_Q)

seq_t a(idx_t n)
{
    idx_t k;
    seq_t sum = 0;

    /* Sum of floor(k * log10(8)) for k = 0..n.
       floor(k * log10(8)) ≈ floor(k * C_R / C_Q) with enough precision. */
    for (k = 0; k <= n; ++k) {
        unsigned __int128 prod = (unsigned __int128)k * C_R;
        seq_t term = (seq_t)(prod / C_Q);
        sum += term;
    }

    /* a(n) = (n + 1) + sum_floor. */
    return (seq_t)(n + 1) + sum;
}
