/*
 * A132104  Number of distinct Tsuro tiles which are square and have Q points
 *          per side.
 * https://oeis.org/A132104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( F_id + 2*F_r90 + 3*F_r180 + 2*F_h ) / 8 where F_id =
 *            (4n-1)!! (product of all odd numbers up to 4n-1) F_r90 = φ₄(n) =
 *            Σ_{k=0}^{⌊n/2⌋} 2^k * n! / ((n-2k)! * k!) F_r180 = φ₂(2n) =
 *            Σ_{k=0}^{⌊n⌋} 2n! / ((2n-2k)! * k!) F_h = φ₂(2n) if n even, else
 *            φ₂(2n-1) (FOIL: identity, two 90° rotations,...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   phi_four, phi_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi_four.h"
#include "../utils/phi_two.h"
/* Compute (4n-1)!! by multiplying the odd numbers 1,3,5,...,4n-1 */
static seq_t odd_double_factorial(idx_t n)
{
    seq_t prod = 1;
    /* 4n-1 has 2n odd factors */
    for (idx_t i = 0; i < 2*n; ++i)
    {
        prod *= (seq_t)(2*i + 1);
    }
    return prod;
}

seq_t a(idx_t n)
{
    seq_t F_id   = odd_double_factorial(n);          /* (4n-1)!!          */
    seq_t F_r90  = phi_four(n);                      /* φ₄(n)             */
    seq_t F_r180 = phi_two(2*n);                     /* φ₂(2n)            */

    /* Reflection over horizontal/vertical axis */
    seq_t F_h;
    if ((n & 1) == 0)       /* n even */
        F_h = F_r180;
    else                    /* n odd  */
        F_h = phi_two(2*n - 1);

    /* Burnside sum: identity + two 90° rotations + one 180° rotation +
                     two axial reflections + two diagonal reflections */
    seq_t total = F_id;
    total += F_r90 * (seq_t)2;
    total += F_r180 * (seq_t)3;
    total += F_h * (seq_t)2;

    return total / (seq_t)8;
}
