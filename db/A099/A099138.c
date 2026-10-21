/*
 * A099138  a(n) = 6^(n-1)*J(n), where J(n) = A001045(n).
 * https://oeis.org/A099138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (12^n - (-6)^n)/18
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Used recurrence: a(0)=0, a(1)=1,  for n>=2  a(n)=6*a(n-1)+72*a(n-2) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev1 = (seq_t)1;   /* a(1) */
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* Use 128‑bit intermediate to avoid overflow up to the limit that fits in seq_t */
        cur = (seq_t)(((__uint128_t)6 * prev1) + (__uint128_t)72 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
