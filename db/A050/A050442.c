/*
 * A050442  Octahedral torus number: a(n) = n^2 + 2*(Sum_{k=1..n-1} k^2) -
 *          2*(floor((n+1)/2)^2 + 2*(Sum_{k=1..floor((n+1)/2)-1} k^2)) + (1 -
 *          (-1)^n)/2.
 * https://oeis.org/A050442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n^2 + 2*∑_{k=1}^{n-1}k^2 - 2*(⌊(n+1)/2⌋^2 +
 *            2*∑_{k=1}^{⌊(n+1)/2⌋-1}k^2) + (1-(-1)^n)/2 (Equivalently a(n) =
 *            (2n^3+n)/3 - (2/3)(2⎣(n+1)/2⎦^3+⎣(n+1)/2⎦) + (1-(-1)^n)/2)
 * Range:     u8 n <= 7, u16 n <= 51, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sum_sq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sum_sq.h"

seq_t a(idx_t n) {
    // n >= 1
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;               // n^2
    seq_t sum1 = sum_sq(n - 1);       // ∑_{k=1}^{n-1} k^2
    seq_t term1 = 2 * sum1;           // 2*sum1

    idx_t f = (n + 1) / 2;            // floor((n+1)/2)
    seq_t ff = (seq_t)f;
    seq_t f2 = ff * ff;               // f^2
    seq_t sum2 = sum_sq(f - 1);       // ∑_{k=1}^{f-1} k^2
    seq_t term2 = 2 * (f2 + 2 * sum2); // 2*(f^2 + 2*sum2)

    seq_t res = n2 + term1;
    res -= term2;
    res += (n % 2);    // (1-(-1)^n)/2 equals n%2

    return res;
}
