/*
 * A211917  Number of (w,x,y,z) with all terms in {1,...,n} and w*x<3*y*z.
 * https://oeis.org/A211917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes
 * a(n) = #{(w,x,y,z) | 1 ≤ w,x,y,z ≤ n and w*x < 3*y*z } .
 *
 * Algorithm outline:
 *   • Let T = floor(w*x/3).  The condition w*x ≥ 3*y*z is equivalent to
 *     y*z ≤ T.
 *   • Pre‑compute for every integer t ≤ floor(n²/3) the number
 *     F[t] = #{(y,z) | 1 ≤ y,z ≤ n , y*z ≤ t}.
 *   • For each product p = w*x (with 1 ≤ w,x ≤ n) add
 *     F[floor(p/3)] to the count of tuples satisfying w*x ≥ 3*y*z.
 *   • a(n) = n⁴ – count_of_complement_tuples.
 *
 * The routine is O(n²) time with a few optimisations and uses only
 * 4*(⌊n²/3⌋+1) bytes of additional memory (≈130 MB for n = 10 000).
 * It works for any external integer type seq_t (including arbitrary‑
 * precision) and index type idx_t (uint64_t).  All arithmetic on seq_t
 * is done using only the allowed operators.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* --------- pre‑compute F[t] = number of (y,z) with y*z ≤ t --------- */
    uint64_t n64 = (uint64_t)n;
    uint64_t maxT = (n64 * n64) / 3ULL;              /* floor(n²/3) */

    size_t sz = (size_t)maxT + 1;
    unsigned int *cnt = (unsigned int *)calloc(sz, sizeof(unsigned int));
    if (!cnt)
        return 0;                               /* no memory → give up */

    for (uint64_t y = 1; y <= n64; ++y) {
        uint64_t maxZ = maxT / y;
        if (maxZ > n64)
            maxZ = n64;
        for (uint64_t z = 1; z <= maxZ; ++z) {
            uint64_t prod = y * z;
            ++cnt[(size_t)prod];
        }
    }

    /* prefix sum: cnt[t] becomes F[t] */
    for (size_t i = 1; i < sz; ++i)
        cnt[i] += cnt[i - 1];

    /* --------- count tuples where w*x ≥ 3*y*z (the complement) --------- */
    seq_t comp = 0;
    for (uint64_t w = 1; w <= n64; ++w) {
        for (uint64_t x = 1; x <= n64; ++x) {
            uint64_t prod = w * x;
            uint64_t t = prod / 3ULL;               /* floor(w*x/3) */
            comp += (seq_t)cnt[(size_t)t];
        }
    }

    free(cnt);

    /* --------- compute a(n) = n⁴ – comp -------------------------------- */
    seq_t nn = (seq_t)n;
    seq_t n4 = nn * nn * nn * nn;
    return n4 - comp;
}
