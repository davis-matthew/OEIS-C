/*
 * A055260  Sums of two powers of 9.
 * https://oeis.org/A055260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 18, u32 n <= 64, u64 n <= 229, u128 n <= 860,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A055260
 *  Formula: a(n) = 9^(n - T_n) + 9^(T_n)
 *  where T_n = floor((sqrt(1 + 8n) - 1) / 2)
 *  (i.e., the largest integer m with m(m+1)/2 ≤ n).
 */
#include <stdint.h>
#include <stdbool.h>

/* ---------- local helpers ---------- */

/* Integer square‑root of a 64‑bit value (floor).  */
static idx_t
isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid * mid <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Power of 9^e computed by repeated multiplication.  */
static seq_t
pow9(idx_t e)
{
    seq_t r = (seq_t)1;
    while (e--) r *= (seq_t)9;
    return r;
}

/* ---------- required function ---------- */

seq_t a(idx_t n)
{
    /* triangular index i = floor((sqrt(1+8n)-1)/2) */
    idx_t t = isqrt_u64(8 * n + 1);     /* floor(sqrt(1+8n)) */
    idx_t i = (t - 1) >> 1;             /* integer division by 2 */

    /* j = n - i(i+1)/2 */
    idx_t tri = i * (i + 1) / 2;
    idx_t j = n - tri;

    /* compute the two powers of 9 and return the sum */
    return pow9(i) + pow9(j);
}
