/*
 * A096192  a(n) = Sum_{k=1..n} C(n,k)^4 where C(n,k) is binomial(n,k).
 * https://oeis.org/A096192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} C(n,k)^4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = Σ_{k=1..n} C(n,k)^4.
 * The function works for any unsigned seq_t type (uint8_t … unsigned __int128)
 * and for any idx_t type (e.g. uint64_t).  No floating point or external
 * libraries are required.  The algorithm uses the multiplicative
 * recurrence for binomial coefficients. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t bin = 1;          /* initially C(n,0) */
    seq_t nn  = (seq_t)n;   /* promote n once */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update bin to C(n,k) using the exact division property. */
        bin = (bin * (nn - (seq_t)(k - 1))) / (seq_t)k;

        /* Compute bin^4 by successive squaring: (bin^2)^2. */
        seq_t sq    = bin * bin;   /* bin^2 */
        seq_t fourth = sq * sq;    /* bin^4 */

        sum += fourth;
    }

    return sum;
}
