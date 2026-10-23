/*
 * A020072  a(n) = floor( Gamma(n+3/8)/Gamma(3/8) ).
 * https://oeis.org/A020072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Gamma(n+3/8)/Gamma(3/8) ) == floor( Π_{k=0}^{n-1}
 *            (8k+3) / 8^n )
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
#ifdef __SIZEOF_INT128__
    /* Use 128‑bit intermediate when seq_t is ≤64 bits to avoid overflow. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 prod = 1;
        for (idx_t k = 0; k < n; ++k)
            prod *= (unsigned __int128)(8 * k + 3);
        unsigned long long shift = (unsigned long long)(3 * n);
        if (shift >= 128) return 0;
        prod >>= shift;
        return (seq_t)prod;
    }
#endif
    /* Fallback for arbitrary‑precision or ≥128‑bit seq_t. */
    seq_t prod = 1;
    for (idx_t k = 0; k < n; ++k)
        prod *= (seq_t)(8 * k + 3);
    unsigned long long shift = (unsigned long long)(3 * n);
    return prod >> shift;
}
