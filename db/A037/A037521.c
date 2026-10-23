/*
 * A037521  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2, 1, 0.
 * https://oeis.org/A037521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(4^(n+1)/7)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Many sequence values fit in seq_t only for relatively small n.
       We compute the quotient iteratively, using only 6‑bit arithmetic
       for the remainder modulo 7.  The method guarantees correctness
       whenever the result fits into seq_t, and it runs in O(n) time.  */
    if (n == 0) return 0;         /* offset 1; a(1) corresponds to n=1 */

    idx_t k = n + 1;              /* we need floor(4^k / 7) with k = n+1 */
    seq_t q = 0;                  /* current quotient Q_i = floor(4^i/7) */
    unsigned int r = 1;           /* current remainder r_i = 4^i mod 7   */

    for (idx_t i = 0; i < k; ++i) {
        /* Q_(i+1) = 4*Q_i + floor(4*r_i / 7)           */
        /* r_(i+1) = (4*r_i) mod 7                      */
        q = (seq_t)4 * q + (seq_t)((4 * r) / 7);
        r = (4 * r) % 7;
    }
    return q;
}
