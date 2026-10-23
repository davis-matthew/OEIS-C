/*
 * A124285  Number of integer-sided pentagons having perimeter n.
 * https://oeis.org/A124285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer‑sided pentagons with perimeter n,
 *            counting rotations and reversals only once. The function
 *            computes the sequence by Burnside's lemma on the dihedral group
 *            D5
 * Range:     u8 n <= 11, u16 n <= 40, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t binom4(idx_t n)
{
    if (n < 4) return 0;
    seq_t r = 1;
    for (idx_t i = 1; i <= 4; ++i)
    {
        r = r * (n - 4 + i) / i;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Maximum side length allowed (since largest side < n/2). */
    idx_t M = (n - 1) / 2;          /* floor((n-1)/2) */
    idx_t k = 5;                    /* number of sides */

    /* ---------- Fixed by identity (all 5‑tuples) ---------- */
    seq_t Fix_id = 0;
    const int C5[6] = {1,5,10,10,5,1};  /* binomial coeffs C(5,j)              */
    for (int j = 0; j <= 5; ++j)
    {
        /* inclusion–exclusion term:
           number of solutions with at most j parts exceeding M.         */
        int64_t top_signed =
            (int64_t)n - (int64_t)j * (int64_t)M - 1;   /* N - j*M - 1       */
        if (top_signed < 4) continue;                /* binom<k-1=4 => 0   */
        idx_t top = (idx_t)top_signed;
        seq_t term = (seq_t)C5[j] * binom4(top);
        if (j & 1)   /* odd j, subtract */
            Fix_id -= term;
        else
            Fix_id += term;
    }

    /* ---------- Fixed by each non‑identity rotation ---------- */
    seq_t Fix_rot = (n % 5 == 0) ? 1 : 0;   /* only when n divisible by 5 */

    /* ---------- Fixed by each reflection ---------- */
    seq_t Fix_ref = 0;
    idx_t half_floor = (n - 1) / 2;          /* floor((n-1)/2)         */
    idx_t S_min = (n - M + 1) / 2;           /* ceil((n-M)/2)          */
    for (idx_t x = 1; x <= M; ++x)
    {
        idx_t S_max = half_floor - x;        /* ceil((n-1)/2) - x      */
        if (S_max < 1) continue;

        idx_t lower = (S_min > x) ? (S_min - x) : 1;
        if (lower < 1) lower = 1;

        idx_t upper = (S_max < M) ? S_max : M;
        if (upper < lower) continue;

        Fix_ref += (upper - lower + 1);
    }

    /* ---------- Burnside's lemma ---------- */
    seq_t total = Fix_id + 4 * Fix_rot + 5 * Fix_ref;
    return total / 10;
}
