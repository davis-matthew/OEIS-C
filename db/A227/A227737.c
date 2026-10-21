/*
 * A227737  n occurs as many times as there are runs in binary representation
 *          of n.
 * https://oeis.org/A227737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k such that Σ_{i=1..k} runs(i) ≥ n, where runs(i)
 *            is the number of runs in the binary representation of i. Thus
 *            each integer m appears exactly runs(m) times in the sequence
 * Range:     u8 n <= 1024, u16 n <= 5120, u32 n <= 5120, u64 n <= 5120,
 *            u128 n <= 5120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>


seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for the offset, but safe */

    idx_t sum = 0;                     /* cumulative number of copies seen */
    for (idx_t k = 1; ; ++k) {
        /* number of runs in k is popcount(k ^ (k >> 1)) */
        idx_t y   = k ^ (k >> 1);
        idx_t cnt = 0;
        while (y) { y &= y - 1; ++cnt; }   /* fast popcount */

        sum += cnt;
        if (sum >= n) return (seq_t)k;     /* first k reaching n gives a(n) */
    }
}
