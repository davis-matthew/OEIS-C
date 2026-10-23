/*
 * A028718  Expansion of
 *          (theta_3(z)*theta_3(7z)*theta_3(49z)+theta_2(z)*theta_2(7z)*theta_2(49z)).
 * https://oeis.org/A028718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9657, u16 n <= 9657, u32 n <= 9657, u64 n <= 9657,
 *            u128 n <= 9657, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A028718
 *  a(n) is the coefficient of q^n in (θ3(z)θ3(7z)θ3(49z)+θ2(z)θ2(7z)θ2(49z)).
 *  An equivalent description is:
 *      a(n) = Σ 2^k
 *  summed over all integer triples (a,b,c) with
 *      a² + 7 b² + 49 c² = n  and  a,b,c all have the same parity,
 *  where k is the number of non‑zero coordinates among (a,b,c) (each
 *  non‑zero coordinate can be chosen with ± sign).
 */

#include <math.h>

/* Integer square root for 64‑bit unsigned input */
static inline idx_t int_sqrt(idx_t x)
{
    idx_t r = (idx_t)sqrt((long double)x);
    while (r * r > x) --r;
    while ((r + 1) * (r + 1) <= x) ++r;
    return r;
}

seq_t a(idx_t n)
{
    seq_t total = 0;

    /* iterate over c ≥ 0 */
    idx_t max_c = int_sqrt(n / 49);
    for (idx_t c = 0; c <= max_c; ++c)
    {
        idx_t rem1 = n - 49 * c * c;
        /* iterate over b ≥ 0 */
        idx_t max_b = int_sqrt(rem1 / 7);
        for (idx_t b = 0; b <= max_b; ++b)
        {
            idx_t rem2 = rem1 - 7 * b * b;
            /* a² must equal rem2 */
            idx_t a_val = int_sqrt(rem2);
            if (a_val * a_val != rem2) continue;

            /* parity test: a,b,c all even or all odd */
            if (((a_val & 1) != (b & 1)) || ((b & 1) != (c & 1)))
                continue;

            /* count non‑zero coordinates to compute sign choices */
            int k = 0;
            if (a_val > 0) ++k;
            if (b > 0)     ++k;
            if (c > 0)     ++k;

            total += ((seq_t)1) << k;
        }
    }
    return total;
}
