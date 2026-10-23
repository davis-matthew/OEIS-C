/*
 * A160473  The p(n) sequence that is associated with the Eta triangle
 *          A160464.
 * https://oeis.org/A160473
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(-(2*(n-1)-1-floor(log(n-1)/log(2))))*(2*n-1)! , n ≥ 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* the function assumes n >= 2; behavior below the offset is irrelevant */
    if (n < 2) return 0;                  /* defensive fallback */

    /* floor_log2(n-1) computed without floating point */
    idx_t lg = 0;
    idx_t tmp = n - 1;
    while (tmp > 1) { tmp >>= 1; ++lg; }

    /* exponent of 2 that must be removed from (2n-1)! */
    idx_t exp_needed = 2 * n - 3 - lg;     /* 2*(n-1)-1 - floor(log2(n-1)) */

    /* compute the result by cancelling 2‑factors as early as possible */
    seq_t product = 1;
    idx_t limit = 2 * n - 1;
    for (idx_t i = 1; i <= limit; ++i) {
        idx_t part = i;

        /* strip powers of 2 from part until either part becomes odd
           or we have cancelled all required 2’s. */
        while ((part & 1) == 0 && exp_needed > 0) {
            part >>= 1;
            --exp_needed;
        }

        product *= (seq_t)part;
    }

    /* any remaining 2‑factors (unlikely) are removed here */
    while (exp_needed > 0) {
        product >>= 1;
        --exp_needed;
    }

    return product;
}
