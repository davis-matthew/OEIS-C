/*
 * A029679  Theta series of quadratic form with Gram matrix [ 2, 0, 0; 0, 2,
 *          1; 0, 1, 4 ].
 * https://oeis.org/A029679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (x1,x2,x3) to x1^2 + x2^2 +
 *            x2*x3 + 2*x3^2 = n. The sequence is the theta series of the
 *            quadratic form with Gram matrix ⎡2 0 0⎤ ⎢0 2 1⎥ ⎣0 1 4⎦ after
 *            dividing the form by 2
 * Range:     u8 n <= 376, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root (floor) for unsigned 64‑bit values. */
static idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        /* mid*mid fits in 128 bits but is small for our typical n. */
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t four_n = n * ((idx_t)4);          /* 4·n */

    /* maximal |x1|:  x1^2 ≤ n  */
    idx_t max_x1 = isqrt_idx(n);

    for (int64_t x1 = -(int64_t)max_x1; x1 <= (int64_t)max_x1; ++x1) {
        idx_t x1sq = (idx_t)((int64_t)x1 * (int64_t)x1);
        idx_t R1   = four_n - (4 * x1sq);   /* 4·n – 4·x1^2 */

        if (R1 < 0) continue; /* theoretical guard */

        /* maximal |x3|: 7·x3^2 ≤ R1 */
        idx_t max_x3 = 0;
        if (R1 >= 7) max_x3 = isqrt_idx(R1 / 7);
        for (int64_t x3 = -(int64_t)max_x3; x3 <= (int64_t)max_x3; ++x3) {
            idx_t x3sq = (idx_t)((int64_t)x3 * (int64_t)x3);
            idx_t d    = R1 - (7 * x3sq);     /* discriminant  Δ = 4·n – 7·x3^2 – 4·x1^2 */

            if (d < 0) continue;

            idx_t s = isqrt_idx(d);
            if (s * s != d) continue;        /* not a perfect square */

            /* parity condition:  s ≡ x3 (mod 2) */
            if ( (s & 1) != ((uint64_t)x3 & 1) ) continue;

            /* number of integer solutions for x2 */
            seq_t add = (s == 0) ? 1 : 2;
            total += add;
        }
    }
    return total;
}
