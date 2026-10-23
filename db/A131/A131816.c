/*
 * A131816  Triangle read by rows: A130321 + A059268 - A000012 as infinite
 *          lower triangular matrices, where A130321 = (1; 2,1; 4,2,1; ...),
 *          A059268 = (1; 1,2; 1,2,4; ...) and A000012 = (1; 1,1; 1,1,1; ...).
 * https://oeis.org/A131816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^m + 2^(r‑m) – 1 * where r = ⌊(√(8n+1) – 1)/2⌋ (row
 *            index), * m = n – r(r+1)/2 (column index)
 * Range:     u8 n <= 35, u16 n <= 135, u32 n <= 527, u64 n <= 2079,
 *            u128 n <= 7380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>

static inline seq_t pow2_exp(idx_t exp)
{
    seq_t v = 1;
    for (idx_t i = 0; i < exp; ++i)
        v *= (seq_t)2;
    return v;
}

seq_t a(idx_t n)
{
    /* find row r using the closed‑form approximation,
       then correct by one if necessary */
    double d = sqrt((double)8 * (double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) * 0.5);

    /* adjust r downward until r(r+1)/2 <= n,
       then adjust upward if needed (protects against
       floating‑point rounding errors) */
    while (((unsigned __int128)(r + 1) * (r + 2) / 2) <= n)
        ++r;
    while (((unsigned __int128)r * (r + 1) / 2) > n)
        --r;

    /* column index m */
    idx_t offset = (unsigned __int128)r * (r + 1) / 2;
    idx_t m = n - offset;

    /* compute 2^m + 2^(r‑m) – 1 safely */
    seq_t p1 = pow2_exp(m);
    seq_t p2 = pow2_exp(r - m);
    return p1 + p2 - (seq_t)1;
}
