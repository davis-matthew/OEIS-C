/*
 * A194749  Number of k such that {-k*sqrt(6)} > {-n*sqrt(6)}, where { } =
 *          fractional part.
 * https://oeis.org/A194749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = #{ k : 1 ≤ k < n ,  { -k*sqrt(6) } > { -n*sqrt(6) } } */
/* Equivalent integer test: let F(x)=floor(sqrt(6*x*x)).
   Then floor_n = F(n), floor_k = F(k),
   and k contributes iff  floor_n - floor_k <= F(n-k). */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n <= 1u)                /* a(1)=0 and a(0) is undefined */
        return 0;

    /* floor_n = ⌊√(6 n²)⌋ = ⌊n·√6⌋  */
    seq_t floor_n = floor_sqrt( (seq_t)6 * ((seq_t)n) * n );

    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        /* floor_k = ⌊√(6 k²)⌋ = ⌊k·√6⌋ */
        seq_t floor_k = floor_sqrt( (seq_t)6 * ((seq_t)k) * k );

        seq_t diff_floor = floor_n - floor_k;
        idx_t d = n - k;                     /* positive */
        seq_t limit = floor_sqrt( (seq_t)6 * ((seq_t)d) * d );

        if (diff_floor <= limit)
            ++count;
    }
    return count;
}
