/*
 * A166453  Triangle read by rows, square of Sierpinski's gasket, (A047999)^2.
 * https://oeis.org/A166453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS Sequence: A166453
 * Formula: Let n = r*(r+1)/2 + k (0 ≤ k ≤ r).  
 *          a(n) = 0               if C(r,k) is even,
 *          a(n) = 2^{popcount(r XOR k)}  if C(r,k) is odd.
 *          (A047999 is the Sierpinski triangle mod 2; its squares give the
 *          described triangle read by rows.)
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    double d = sqrt((double)n * 8.0 + 1.0);
    idx_t r = (idx_t)((d - 1.0) * 0.5);
    /* Correct possible rounding errors */
    while ((unsigned __int128)r * (r + 1) / 2 > n) r--;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) r++;

    idx_t k = n - (idx_t)((unsigned __int128)r * (r + 1) / 2);

    /* C(r,k) is odd iff (k & (r - k)) == 0 (no carries in binary addition) */
    if ((k & (r - k)) != 0) {
        return 0;
    }

    /* a(n) = 2 ^ popcount(r XOR k) */
    unsigned int pop = __builtin_popcountll(r ^ k);
    return ((seq_t)1) << pop;
}
