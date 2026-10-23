/*
 * A024500  a(n) = [ C(2n,n)/n^2 ].
 * https://oeis.org/A024500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( C(2n,n) / n^2 )
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the central binomial coefficient C(2n,n) multiplicatively
 * and divides by n^2.  For 64‑bit builds the intermediate
 * expression is performed in an unsigned __int128 type to avoid
 * overflow when the binomial coefficient does not fit in 64 bits
 * but the final quotient does.  For larger seq_t (arbitrary‑precision)
 * the arithmetic is done on seq_t directly. */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for the offset, but return 0 safely */
        return 0;

#ifdef __SIZEOF_INT128__
    /* 64‑bit integral seq_t : use __int128 intermediate */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned __int128 binom = 1;
        for (idx_t i = 1; i <= n; ++i) {
            binom = binom * (unsigned __int128)(n + i);
            binom /= i;            /* exact division at each step */
        }
        unsigned __int128 denom = (unsigned __int128)n * (unsigned __int128)n;
        return (seq_t)(binom / denom);
    }
#endif

    /* arbitrary‑precision seq_t (or 128‑bit) */
    seq_t binom = 1;
    for (idx_t i = 1; i <= n; ++i) {
        binom = binom * ((seq_t)(n + i));
        binom /= i;                /* exact division at each step */
    }
    seq_t denom = (seq_t)n * (seq_t)n;
    return binom / denom;
}
