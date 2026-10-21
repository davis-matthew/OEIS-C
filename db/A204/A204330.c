/*
 * A204330  a(n) is the number of k satisfying 1 <= k <= n and such that
 *          floor(sqrt(k)) divides k.
 * https://oeis.org/A204330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(2*sqrt(n)) + floor(sqrt(n-1)) - 1 if n in A135106
 *            otherwise a(n) = floor(2*sqrt(n)) + floor(sqrt(n-1)) - 2 where
 *            A135106 is the set of n such that floor(sqrt(n)) divides n
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = ((idx_t)1 << 32);   /* upper bound: ceil sqrt(2^64)  */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid > 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            low = mid;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* out of range, not used for offset 1 */
    idx_t s = isqrt_u64(n);            /* floor(sqrt(n))              */
    idx_t t = isqrt_u64(n - 1);        /* floor(sqrt(n-1))            */
    idx_t add = (n >= s * s + s + 1) ? 1 : 0;   /* decide +1 for floor(2*sqrt(n)) */
    seq_t base = (seq_t)(2 * s + add + t);

    /* n is in A135106 iff floor(sqrt(n)) divides n */
    if (n % s == 0) {
        return base - 1;
    } else {
        return base - 2;
    }
}
