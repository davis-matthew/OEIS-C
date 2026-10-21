/*
 * A053615  Pyramidal sequence: distance to nearest product of two consecutive
 *          integers (promic or heteromecic numbers).
 * https://oeis.org/A053615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = | n - t·(t+1) | where t = ⌊√n⌋
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer floor square‑root for unsigned 64‑bit numbers.
   Uses only integer division, safe for all n ≥ 0. */
static idx_t floor_sqrt_u64(idx_t n)
{
    if (n == 0)          /* avoid division by zero */
        return 0;

    idx_t lo = 1, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        /* mid <= n/mid  ⇔  mid² ≤ n   (no overflow) */
        if (mid <= n / mid)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;          /* hi is the greatest integer with hi² ≤ n */
}

seq_t a(idx_t n)
{
    idx_t t    = floor_sqrt_u64(n);    /* t = ⌊√n⌋ */
    seq_t tn   = (seq_t)t;
    seq_t prod = tn * ((seq_t)1 + tn); /* t·(t+1)  */
    seq_t nseq = (seq_t)n;

    return (nseq >= prod) ? (nseq - prod) : (prod - nseq); /* |n - prod| */
}
