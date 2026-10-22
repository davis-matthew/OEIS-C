/*
 * A006376  Number of indecomposable positive definite ternary quadratic forms
 *          of determinant n.
 * https://oeis.org/A006376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search. a(n) = T(n) - D(n), where T(n) = number of
 *            Eisenstein-reduced classically integral ternary forms a x^2+b
 *            y^2+c z^2+r yz+s xz+t xy (r,s,t even) of determinant n (one per
 *            isometry class), D(n) = #{x<=y<=z : x*y*z = n} + Sum_{k|n}
 *            B(n/k) (decomposable classes), B(m) = number of reduced binar...
 * Range:     u8 n <= 250, u16 n <= 250, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   binary_indec, ternary_total
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binary_indec.h"
#include "../utils/ternary_total.h"

seq_t a(idx_t n)
{
    long long N = (long long)n, x, y, k;
    long long total = ternary_total(N);
    long long dec = 0;

    for (x = 1; x * x * x <= N; x++) {
        long long rem;
        if (N % x) continue;
        rem = N / x;
        for (y = x; y * y <= rem; y++) {
            if (rem % y == 0) dec++;
        }
    }
    for (k = 1; k <= N; k++) {
        if (N % k == 0) dec += binary_indec(N / k);
    }
    return (seq_t)(total - dec);
}
