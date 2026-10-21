/*
 * A212052  Number of (w,x,y,z) with all terms in {1,...,n} and 4w*x=5*y*z.
 * https://oeis.org/A212052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = |{(w,x,y,z) : 1 ≤ w,x,y,z ≤ n, 4*w*x = 5*y*z}|
 *
 * The program first counts, for every product t (1 ≤ t ≤ n²),
 * the number of ordered pairs (a,b) with a*b = t and a,b ≤ n.
 * These counts are stored in the array f[ ], where
 *   f[t] = #{(a,b) : 1 ≤ a,b ≤ n, a*b = t} .
 *
 * For each m that is a multiple of 5, we have
 *   4*w*x = 5*y*z   ⇔   4*m = 5*t   with t = y*z .
 * Hence y*z = 4*m/5 and we only need t that is an integer.
 * The total number of solutions is therefore
 *     Σ_{m ≤ n², 5|m} f[m] * f[4*m/5] .
 *
 * Complexity:  O(n²) time for building f, plus O(n²/5) for the sum.
 * Memory:  ~2·n² bytes using uint16_t (≈200 MiB for n = 10 000).
 *
 * This implementation uses dynamic allocation and frees memory
 * before returning the result.
 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset: a(0) = 0 */
    if (n == 0)
        return 0;

    idx_t nn = n;
    uint64_t maxProd = (uint64_t)nn * nn;         /* n² */

    /* Allocate array f[0 .. n²] of uint16_t counts */
    size_t len = (size_t)maxProd + 1;
    uint16_t *f = (uint16_t *)calloc(len, sizeof(uint16_t));
    if (!f)   /* allocation failed */
        return 0;

    /* Count ordered pairs (a,b) with product t */
    for (idx_t a = 1; a <= nn; ++a) {
        uint64_t aa = (uint64_t)a;
        for (idx_t b = 1; b <= nn; ++b) {
            uint64_t prod = aa * (uint64_t)b;
            ++f[(size_t)prod];
        }
    }

    /* Sum over multiples of 5 */
    seq_t ans = 0;
    for (uint64_t m = 5; m <= maxProd; m += 5) {
        uint64_t k = (4 * m) / 5;          /* integer because 5|m */
        seq_t c1 = f[(size_t)m];
        seq_t c2 = f[(size_t)k];
        if (c1 && c2)
            ans += c1 * c2;
    }

    free(f);
    return ans;
}
