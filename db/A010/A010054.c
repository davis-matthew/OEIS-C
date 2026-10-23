/*
 * A010054  a(n) = 1 if n is a triangular number, otherwise 0.
 * https://oeis.org/A010054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 if n is a triangular number, otherwise 0
 * Range:     u8 n <= 9, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Check whether a non‑negative integer is a perfect square.
   Uses binary search on the (possibly arbitrary‑precision) type seq_t. */
static seq_t isqrt(seq_t x)
{
    seq_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        /* mid = (lo + hi) / 2;  avoid overflow on some big‑int types: */
        seq_t mid = (lo + hi) / 2;
        seq_t sq  = mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* 8*n + 1 as seq_t to avoid overflow in small types. */
    seq_t t = (seq_t)n;
    t = t * 8 + 1;
    seq_t s = isqrt(t);
    /* n is triangular iff 8*n+1 is an odd perfect square. */
    return (s * s == t && (s & 1) == 1) ? (seq_t)1 : (seq_t)0;
}
