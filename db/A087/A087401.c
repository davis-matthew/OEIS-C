/*
 * A087401  Triangle of n*r-binomial(r+1,2).
 * https://oeis.org/A087401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = k * (2*r - k - 1) / 2 where r = floor((sqrt(8*n+1)-1)/2),
 *            k = n - r*(r+1)/2 (T(r,k) = sum_{i=1}^{r} min(k,i-1) for 0≤k≤r)
 * Range:     u8 n <= 298, u16 n <= 7380, u32 n <= 7380, u64 n <= 7380,
 *            u128 n <= 7380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>   /* for sqrtl, we use long double for extra precision */

seq_t a(idx_t n) {
    /* Find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t lo = 0, hi = (n > 0 ? n : 0); /* upper bound */
    idx_t r = 0;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;  /* mid*(mid+1)/2 safely */
        if (t <= n) {
            r = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break; /* avoid underflow */
            hi = mid - 1;
        }
    }

    /* Compute k = n - r(r+1)/2 */
    idx_t tri = (idx_t)((__int128)r * (r + 1) / 2);
    idx_t k = n - tri;

    /* Apply formula: T(r,k) = k*(2*r - k - 1)/2 */
    seq_t kseq = (seq_t)k;
    seq_t rseq = (seq_t)r;
    seq_t tnum = kseq * ((seq_t)2 * rseq - kseq - (seq_t)1);
    return tnum / (seq_t)2;
}
