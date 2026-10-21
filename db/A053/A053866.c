/*
 * A053866  Parity of A000203(n), the sum of the divisors of n; a(n) = 1 when
 *          n is a square or twice a square, 0 otherwise.
 * https://oeis.org/A053866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if n is a square or twice a square, 0 otherwise
 * Range:     u8 n <= 9666, u16 n <= 9666, u32 n <= 9666, u64 n <= 9666,
 *            u128 n <= 9666, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square‑root for unsigned 64‑bit values.  Returns ⌊√n⌋.
   Uses a binary search that stays within unsigned arithmetic. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    if (n < 2) return n;          /* 0 → 0, 1 → 1 */
    idx_t low  = 1;
    idx_t high = (n >> 1) + 1;    /* √n ≤ n/2 + 1 for n ≥ 2 */
    while (low <= high) {
        idx_t mid  = low + ((high - low) >> 1);
        idx_t div  = n / mid;     /* avoid overflow in mid*mid */
        if (mid <= div) {
            low = mid + 1;
        } else {
            if (mid == 0) break; /* safety for underflow, not expected */
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    /* test if n is a perfect square */
    idx_t r = floor_sqrt_u64(n);
    if (r * r == n) return (seq_t)1;

    /* test if n is twice a perfect square */
    if ((n & 1ULL) == 0) {                /* n even */
        idx_t m = n >> 1;                 /* m = n/2 */
        idx_t s = floor_sqrt_u64(m);
        if (s * s == m) return (seq_t)1;
    }
    return (seq_t)0;
}
