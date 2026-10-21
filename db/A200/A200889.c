/*
 * A200889  Number of 0..n arrays x(0..5) of 6 elements without any interior
 *          element greater than both neighbors.
 * https://oeis.org/A200889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 63, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for A200889:
   a(n) = Σ_{i=0}^{6} p_i * C(n+5-i, 6)
   where p = {37, 33, 1, -54, 21, -7, 1}  and C(m,6) is the binomial coefficient. */
#include <stdint.h>

/* Helper: compute C(N,6) for N ≥ 0, return 0 if N < 6.  Intermediate arithmetic
   performed in signed __int128 to avoid overflow for the 64‑bit build. */
static inline seq_t binom6(idx_t N) {
    if (N < 6) return 0;
    __int128 r = 1;
    for (int i = 1; i <= 6; ++i) {
        r = r * (N - 6 + i) / i;
    }
    return (seq_t)r;
}

/* Main sequence function.  Uses the explicit binomial formula, which is a
   closed‑form expression of the 7‑term linear recurrence given in OEIS. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1; should not be called with 0 */
    const int p[7] = {37, 33, 1, -54, 21, -7, 1};
    __int128 sum = 0;
    for (int i = 0; i < 7; ++i) {
        idx_t N = n + 5 - i;       /* n+5-i = (n-1-i)+6 */
        if (N < 6) continue;
        seq_t bin = binom6(N);
        if (bin == 0) continue;
        sum += (__int128)bin * p[i];
    }
    return (seq_t)sum;
}
