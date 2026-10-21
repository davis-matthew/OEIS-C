/*
 * A095805  Reduced numerators in Wolfram's iteration for sqrt(2).
 * https://oeis.org/A095805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Reduced numerators from Wolfram's iteration for sqrt(2).
   a(0) = 0.
   For n ≥ 1 set r₀ = 1 and
        r_{k+1} = 2·r_k + e_k  (k ≥ 0),
   where the binary digit e_k is
        e_k = 1  if (2·r_k + 1)² ≤ 2^(2k+3),
           0  otherwise.
   Then a(n) is r_{n-1} with all factors of 2 removed
   (i.e. divided by 2^{v₂(r_{n-1})}). */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* r_k will hold r_{k} during the loop. */
    seq_t r = 1;                     /* r₀ = 1 */
    idx_t kmax = n - 1;              /* need r_{kmax} */

    for (idx_t k = 0; k < kmax; ++k)
    {
        /* Compute e_k using an unsigned 128‑bit temporary. */
        unsigned __int128 w = ((unsigned __int128)r << 1) + 1;   /* 2·r + 1 */
        unsigned __int128 w_sq = w * w;                       /* (2·r + 1)² */
        idx_t shift = 2 * k + 3;                              /* exponent of 2 */

        seq_t e;
        if (shift < 128) {
            unsigned __int128 thresh = ((unsigned __int128)1) << shift;
            e = (w_sq <= thresh) ? 1 : 0;
        } else {
            /* Shift larger than 127 cannot be represented in 128 bits;
               for such k the threshold exceeds any representable value
               and the inequality holds, so e = 1. */
            e = 1;
        }

        r = (r << 1) + e;   /* r_{k+1} */
    }

    /* Remove all factors of 2 from r to obtain the reduced numerator. */
    while ((r & 1) == 0) r >>= 1;

    return r;
}
