/*
 * A091594  Triangle read by rows: T(n,m) := Sum_{k=0..floor((n-m)/2)}
 *          binomial(n-2k,m) * binomial(n-m-k,k).
 * https://oeis.org/A091594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 140, u32 n <= 474, u64 n <= 1728,
 *            u128 n <= 6479, big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A091594 – Triangle read by rows
    a(n) = T(row,col)
    row = floor((sqrt(8n+1)-1)/2)
    col = n - row*(row+1)/2
    T(n,m) = Σ_{k=0..⌊(n-m)/2⌋} binomial(n-2k,m) * binomial(n-m-k,k)
 */
#include <math.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    /* Find row index r */
    long double discr = sqrtl(8.0L * (long double)n + 1.0L);
    idx_t r = (idx_t)((discr - 1.0L) / 2.0L);
    /* Correct rounding errors */
    while ((__uint128_t)r * (r + 1) / 2 > n) r--;
    while ((__uint128_t)(r + 1) * (r + 2) / 2 <= n) r++;
    /* Column index */
    idx_t tri = (idx_t)((__uint128_t)r * (r + 1) / 2);
    idx_t m = n - tri;
    /* Compute T(r,m) */
    idx_t maxk = (r > m) ? ((r - m) / 2) : 0;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t c1 = binomial(r - 2 * k, m);
        seq_t c2 = binomial(r - m - k, k);
        seq_t prod = seq_mul(c1, c2);
        sum += prod;
    }
    return sum;
}
