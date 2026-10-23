/*
 * A024685  a(n) = s(1)s(n) + s(2)s(n-1) + ... + s(k)s(n+1-k), where k = [
 *          (n+1)/2 ], s = A000201 (lower Wythoff sequence).
 * https://oeis.org/A024685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} ⌊i·φ⌋ · ⌊(n+1-i)·φ⌋
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (φ = (1+√5)/2 is the golden ratio, and ⌊x⌋ denotes the floor)   */

#include <stdint.h>

/* Integer square root (floor) for 64‑bit unsigned values.  */
/* No floating point or math.h functions are used.        */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x, res = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return res;
}

/* Lower Wythoff sequence A000201: ⌊n·φ⌋  */
/* Computed by the identities:             */
/*   n·φ = (n + n·√5) / 2  (exact)         */
/*   ⌊n·√5⌋ = ⌊√(5·n²)⌋  (integer sqrt)  */
static inline seq_t lower_wythoff(idx_t n)
{
    /* 5 * n * n  (must be done in 128‑bit to avoid overflow) */
    unsigned __int128 prod = (unsigned __int128)5 * n * n;
    idx_t srt = isqrt_u64((idx_t)prod);         /* floor(√(5·n²))  */
    return ((seq_t)n + (seq_t)srt) / 2;          /* floor(n·φ)      */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined only for n ≥ 1 in OEIS */
    idx_t k = (n + 1) / 2;         /* floor((n+1)/2) */
    seq_t sum = 0;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t li = lower_wythoff(i);
        seq_t lj = lower_wythoff(n + 1 - i);
        sum += li * lj;
    }
    return sum;
}
