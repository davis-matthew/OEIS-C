/*
 * A053616  Pyramidal sequence: distance to nearest triangular number.
 * https://oeis.org/A053616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = min(|n - k(k+1)/2|, |n - (k+1)(k+2)/2|) where k =
 *            floor((sqrt(8n+1)-1)/2)
 * Range:     u8 n <= 396, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the distance from n to the nearest triangular number. */

static inline idx_t isqrt_u64(idx_t x)
{
    /* Integer square root for 64‑bit values, using the method from HAKMEM. */
    idx_t r = 0;
    idx_t bit = ((idx_t)1) << 62;   /* highest power of four <= x */
    while (bit > x) bit >>= 2;
    while (bit)
    {
        if (x >= r + bit)
        {
            x -= r + bit;
            r = (r >> 1) + bit;
        }
        else
            r >>= 1;
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n == 0) return 0;

    /* Compute floor(sqrt(8*n+1)) safely */
    idx_t m = 8*n + 1;
    idx_t s = isqrt_u64(m);

    /* k = floor((s-1)/2) */
    idx_t k = (s - 1) / 2;

    /* Triangular numbers around k */
    seq_t t1 = (seq_t)k * (k + 1) / 2;
    seq_t t2 = (seq_t)(k + 1) * (k + 2) / 2;

    /* Distances */
    seq_t d1 = n > t1 ? (seq_t)(n - t1) : (seq_t)(t1 - n);
    seq_t d2 = n > t2 ? (seq_t)(n - t2) : (seq_t)(t2 - n);

    return d1 < d2 ? d1 : d2;
}
