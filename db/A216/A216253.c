/*
 * A216253  A213196 as table read layer by layer - layer clockwise, layer
 *          counterclockwise and so on.
 * https://oeis.org/A216253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 131, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multi-dimensional pairing with boustrophedonic layers
   A213196: permutation of natural numbers
   Formula: a(n) = (m1+m2-1)*(m1+m2-2)/2 + m1
            where
              m1 = (3*i + j - 1 - (-1)^i + (i+j-2)*(-1)^(i+j)) / 4
              m2 = ((1+(-1)^i) * ((1+(-1)^j)*2*int((j+2)/4) - (-1+(-1)^j)*(2*int((i+4)/4) + 2*int(j/2))) -
                    (-1+(-1)^i) * ((1+(-1)^j)*(1+2*int(i/4)+2*int(j/2)) - (-1+(-1)^j)*(1+2*int(j/4))))/4
   Layers are read clockwise for odd layers and counter‑clockwise for even layers.
   Each layer t contains 2*t-1 new cells.
*/

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) */
static idx_t u64_floor_sqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid > n / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* just a safeguard – offset is 1 */

    /* Find layer t: smallest t with t^2 >= n  --> t = floor_sqrt(n-1)+1 */
    idx_t t = u64_floor_sqrt(n - 1) + 1;

    /* Number of cells before this layer */
    idx_t prev_sq = (t - 1) * (t - 1);
    idx_t k = n - prev_sq;          /* 1 <= k <= 2*t-1 */

    /* Coordinates (i,j) in the matrix (1‑based) */
    idx_t i, j;
    if (t & 1) {                    /* odd layer: clockwise */
        if (k <= t) {
            i = k;
            j = t;
        } else {
            i = t;
            j = 2 * t - k;
        }
    } else {                        /* even layer: counter‑clockwise */
        if (k <= t) {
            i = t;
            j = k;
        } else {
            i = 2 * t - k;
            j = t;
        }
    }

    /* Parities for the formulas */
    long long li = (long long)i;
    long long lj = (long long)j;
    int sign_i = (li & 1) ? -1 : 1;
    int sign_j = (lj & 1) ? -1 : 1;
    int sign_ij = sign_i * sign_j;

    /* Compute m1 */
    long long num1 = 3 * li + lj - 1 - sign_i + (li + lj - 2) * sign_ij;
    seq_t m1 = (seq_t)(num1 / 4);    /* guaranteed divisible by 4 */

    /* Intermediate integer parts used in m2 */
    long long intA = (lj + 2) / 4;
    long long intB = (li + 4) / 4;
    long long intC = lj / 2;
    long long intD = li / 4;
    long long intE = lj / 4;

    /* Compute m2 */
    long long term1 = 1 + sign_i;
    long long term2 = 1 + sign_j;
    long long term3 = 2 * intA;
    long long term4 = -1 + sign_j;
    long long term5 = 2 * intB + 2 * intC;
    long long inner = term2 * term3 - term4 * term5;
    long long product1 = term1 * inner;

    long long term6 = -1 + sign_i;
    long long term7 = 1 + sign_j;
    long long term8 = 1 + 2 * intD + 2 * intC;
    long long term9 = -1 + sign_j;
    long long term10 = 1 + 2 * intE;
    long long inner2 = term7 * term8 - term9 * term10;
    long long product2 = term6 * inner2;

    long long num2 = product1 - product2;
    seq_t m2 = (seq_t)(num2 / 4);    /* guaranteed divisible by 4 */

    /* Cantor pairing */
    return ((m1 + m2 - 1) * (m1 + m2 - 2)) / 2 + m1;
}
