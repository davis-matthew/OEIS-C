/*
 * A003603  Fractal sequence obtained from Fibonacci numbers (or Wythoff
 *          array).
 * https://oeis.org/A003603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, eigen, tabf, changed
 * Formula:   a(n) = row of the Wythoff array that contains n. Write n in
 *            Zeckendorf form (F(2)=1, F(3)=2, ...). Shift every index down
 *            until the smallest term is F(1), which gives x = floor(r*phi).
 *            Then r = floor((x+1)/phi) = (isqrt(5*(x+1)^2) - (x+1)) / 2
 * Range:     u8 n <= 668, u16 n <= 9674, u32 n <= 9674, u64 n <= 9674,
 *            u128 n <= 9674, big any n
 * Helpers:   a3603_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a3603_isqrt.h"

seq_t a(idx_t n) {
    uint64_t F[100];
    int k, top, idx[100], cnt = 0, L, i;
    uint64_t rem = (uint64_t)n, x = 0;
    unsigned __int128 y, s;
    F[0] = 0; F[1] = 1; F[2] = 1;
    top = 2;
    while (top < 93 && F[top] + F[top - 1] <= (uint64_t)n) {
        F[top + 1] = F[top] + F[top - 1];
        top++;
    }
    for (k = top; k >= 2 && rem > 0; k--) {
        if (F[k] <= rem) {
            rem -= F[k];
            idx[cnt++] = k;
            k--; /* Zeckendorf: no two consecutive indices */
        }
    }
    L = idx[cnt - 1];          /* smallest index used, >= 2 */
    for (i = 0; i < cnt; i++) x += F[idx[i] - (L - 1)];
    y = (unsigned __int128)x + 1;
    s = a3603_isqrt(5 * y * y);
    return (seq_t)(uint64_t)((s - y) / 2);
}
