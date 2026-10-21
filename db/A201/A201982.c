/*
 * A201982  Consider the numbers 0 <= n <= 999 whose decimal digits are
 *          represented by (a,b,c). Look at the cross product of the vectors
 *          (u,v,w) = (a,b,c)^(c,b,a) in 3-dimensional Euclidean space.
 *          Sequence gives numbers n such that the components u, v, w are > =
 *          0.
 * https://oeis.org/A201982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the nth smallest integer 0 ≤ x ≤ 999 whose decimal
 *            digits (a,b,c) satisfy the inequalities a*b - b*c ≥ 0 and c*c -
 *            a*a ≥ 0, i.e. the cross product (a,b,c) ^ (c,b,a) has all
 *            non‑negative components
 * Range:     u8 n <= 50, u16 n <= 145, u32 n <= 145, u64 n <= 145,
 *            u128 n <= 145, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t seq[1000];
    static idx_t len = 0;
    static bool built = false;

    if (!built) {
        for (int a = 0; a <= 9; ++a) {
            for (int b = 0; b <= 9; ++b) {
                for (int c = 0; c <= 9; ++c) {
                    int u = a * b - b * c;
                    int v = c * c - a * a;
                    if (u >= 0 && v >= 0) {
                        seq[len++] = (seq_t)(a * 100 + b * 10 + c);
                    }
                }
            }
        }
        built = true;
    }

    if (n == 0 || (n - 1) >= len)
        return (seq_t)0;          /* undefined for out‑of‑range indices */
    return seq[n - 1];
}
