/*
 * A126611  Sum x+y of generator pairs (x, y) {x and y coprime and not both
 *          odd} of primitive Pythagorean triangles, sorted.
 * https://oeis.org/A126611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2k-1) where k is the smallest integer such that *
 *            Σ_{i=1}^{k-1} φ(2i-1)/2 < n ≤ Σ_{i=1}^{k} φ(2i-1)/2. *
 *            Equivalently, each odd integer m = 2k-1 occurs φ(m)/2 times in
 *            the sequence
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euler’s totient: φ(n) */
static idx_t phi_of(idx_t n)
{
    idx_t result = n;
    idx_t tmp = n;
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            result -= result / p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) result -= result / tmp;
    return result;
}

/* Sequence member a(n) for n ≥ 1 (offset 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 1 */
    idx_t m = 3;                         /* first odd generator sum */
    while (1) {
        idx_t cnt = phi_of(m) / 2;        /* multiplicity of m */
        if (n <= cnt) return (seq_t)m;
        n -= cnt;
        m += 2;
    }
}
