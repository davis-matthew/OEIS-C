/*
 * A056344  Number of bracelets of length n using exactly four different
 *          colored beads.
 * https://oeis.org/A056344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   phi, pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 * a(n) = (k!/(2n)) * ( Σ_{d|n} φ(d)·S2(n/d,k) + Reflections ),
 * where k = 4 and S2(m,4) = (4^m - 4·3^m + 6·2^m - 4) / 24.
 *
 * For k = 4, k! = 24, so factor = 12/n.
 *
 * Reflections:
 *   if n is odd : Reflections = n * S2((n+1)/2,4)
 *   if n is even: Reflections = (n/2) * ( S2(n/2,4) + S2(n/2+1,4) )
 *
 * This implements the Burnside‐count for bracelets with exactly four colors.
 */

#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"
#include "../utils/phi.h"

static seq_t S2_4(idx_t m)
{
    if (m < 4)
        return 0;
    seq_t p4 = pow4(m);   /* 4^m */
    seq_t p3 = pow3(m);   /* 3^m */
    seq_t p2 = pow2(m);   /* 2^m */

    seq_t val = p4 - 4 * p3 + 6 * p2 - 4; /* numerator */
    return val / 24;                      /* integer division */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1, unused case */

    /* Sum over rotations: Σ_{d|n} φ(d) * S2(n/d,4) */
    seq_t sum_rot = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;

            seq_t phi_d1 = phi(d1);
            seq_t phi_d2 = phi(d2);

            seq_t s2_d1 = S2_4(n / d1);
            seq_t s2_d2 = S2_4(n / d2);

            if (d1 == d2) {                 /* perfect square */
                sum_rot += phi_d1 * s2_d1;
            } else {
                sum_rot += phi_d1 * s2_d1;
                sum_rot += phi_d2 * s2_d2;
            }
        }
    }

    /* Sum over reflections */
    seq_t sum_ref = 0;
    if (n & 1) { /* odd */
        seq_t s2_half = S2_4((n + 1) / 2);
        sum_ref = (seq_t)n * s2_half;
    } else {     /* even */
        seq_t s2_half = S2_4(n / 2);
        seq_t s2_half_plus = S2_4(n / 2 + 1);
        sum_ref = (n / 2) * (s2_half + s2_half_plus);
    }

    seq_t total = sum_rot + sum_ref;          /* numerator in Burnside formula */
    /* factor = 12 / n */
    seq_t result = (seq_t)12 * total / n;
    return result;
}
