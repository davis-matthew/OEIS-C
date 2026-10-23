/*
 * A194165  a(n) = [Sum_{k=1..n} (k*r)], where [ ]=floor, ( )=fractional part,
 *          and r = (1+sqrt(5))/2, the golden ratio.
 * https://oeis.org/A194165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊ Σ_{k=1}^n (k*φ − ⌊k*φ⌋) ⌋ Equivalent: a(n) =
 *            ⌊φ*n(n+1)/2⌋ – Σ_{k=1}^n ⌊k*φ⌋ where φ = (1+√5)/2 and ⌊x⌋ is
 *            floor(x)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 128, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Compute ⌊k*φ⌋ using integer arithmetic:
   ⌊k*φ⌋ = ⌊ (k + ⌊k*√5⌋) / 2 ⌋,
   and ⌊k*√5⌋ = ⌊√(5*k²)⌋.  */
static seq_t floor_phi(seq_t k)
{
    seq_t sqrt_part = floor_sqrt((seq_t)5 * k * k);
    return (k + sqrt_part) / 2;   /* integer division truncates towards 0 */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; below offset unspecified */

    seq_t n_seq = (seq_t)n;
    seq_t m = n_seq * (n_seq + 1) / 2;     /* m = n(n+1)/2 */
    seq_t total = floor_phi(m);            /* ⌊φ*m⌋ = ⌊φ*n(n+1)/2⌋ */

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
        sum += floor_phi((seq_t)k);        /* Σ ⌊k*φ⌋ */

    return total - sum;                   /* a(n) = ⌊φ*n(n+1)/2⌋ − Σ ⌊k*φ⌋ */
}
