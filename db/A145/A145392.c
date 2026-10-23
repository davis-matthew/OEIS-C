/*
 * A145392  Number of inequivalent sublattices of index n in square lattice,
 *          where two sublattices are considered equivalent if one can be
 *          rotated by a multiple of Pi/2 to give the other.
 * https://oeis.org/A145392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (σ₁(n) + χ(n)) / 2 * where σ₁(n) = Σ_{d|n} d (sum of
 *            divisors) * χ(d) = 0 if d is even; +1 if d ≡ 1 (mod 4); -1 if d
 *            ≡ 3 (mod 4)
 * Range:     u8 n <= 179, u16 n <= 9950, u32 n <= 9950, u64 n <= 9950,
 *            u128 n <= 9950, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation iterates over all divisors d ≤ √n.
 * For each divisor pair (d, n/d) it adds to σ₁ and to χ.
 * The loop uses d ≤ n/d to avoid overflow on n up to 2⁶⁴-1. */

#include <stddef.h>
#include <stdint.h>

/* seq_t and idx_t are provided by the build; no definitions needed. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* Undefined for n=0, return 0 safely. */

    seq_t sigma = 0;                        /* σ₁(n) */
    seq_t chi   = 0;                        /* χ(n) */

    for (idx_t d = 1; d <= n / d; ++d) {    /* iterate d ≤ √n, no overflow */
        if (n % d == 0) {
            idx_t q = n / d;                /* complementary divisor */

            /* Add d to σ₁ and χ */
            sigma += (seq_t)d;
            if (d & 1)                       /* d is odd */
                chi += (d % 4 == 1) ? 1 : -1;

            if (q != d) {                   /* add complementary div. if distinct */
                sigma += (seq_t)q;
                if (q & 1)                   /* q is odd */
                    chi += (q % 4 == 1) ? 1 : -1;
            }
        }
    }

    return (sigma + chi) / 2;
}
