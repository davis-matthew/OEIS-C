/*
 * A049790  Triangular array T read by rows: T(n,k) = Sum_{j=1..k}
 *          floor(n/floor(k/j)).
 * https://oeis.org/A049790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = ceil((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r-1)/2, and T(r,k) = Σ_{j=1..k} floor(r / floor(k/j))
 * Range:     u8 n <= 209, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static idx_t find_row(idx_t n)
{
    /* Compute r = smallest integer such that r*(r+1)/2 >= n */
    double t = sqrt((double)8 * (double)n + 1.0);
    idx_t r = (idx_t)((t - 1.0) * 0.5);
    /* Correct possible under‑ or over‑estimation */
    while ((unsigned __int128)r * (r + 1) / 2 < n) ++r;
    while (r > 0 && (unsigned __int128)(r - 1) * r / 2 >= n) --r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for offset 1, but safe guard */

    idx_t r = find_row(n);
    idx_t prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - prev_tri;

    seq_t res = (seq_t)0;
    seq_t seq_r = (seq_t)r;

    for (idx_t j = 1; j <= k; ++j) {
        idx_t denom = k / j;            /* floor(k/j) */
        seq_t add = seq_r / (seq_t)denom;
        res += add;
    }
    return res;
}
