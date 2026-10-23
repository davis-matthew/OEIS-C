/*
 * A191567  Four interlaced 2nd order polynomials: a(4*k) = k*(1+2*k);
 *          a(1+2*k) = 2*(1+2*k)*(3+2*k); a(2+4*k) = 4*(1+k)*(1+2*k).
 * https://oeis.org/A191567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(4k) = k * (1 + 2k) * a(2+4k) = 4 * (1 + k) * (1 + 2k) * a(2k
 *            + 1) = 2 * (1 + 2k) * (3 + 2k)
 * Range:     u8 n <= 10, u16 n <= 180, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0) {                /* n is even */
        if ((n & 3ULL) == 0) {            /* n % 4 == 0  ->  a(4k) */
            seq_t k = (seq_t)(n / 4);     /* k = n/4                  */
            return k * ((seq_t)1 + ((seq_t)2 * k));
        } else {                          /* n % 4 == 2  ->  a(2+4k) */
            seq_t k = (seq_t)((n - 2) / 4);
            return ((seq_t)4 * ((seq_t)1 + k) *
                    ((seq_t)1 + ((seq_t)2 * k)));
        }
    } else {                              /* n is odd  ->  a(2k+1) */
        seq_t k = (seq_t)((n - 1) / 2);   /* k = (n-1)/2              */
        return ((seq_t)2 * ((seq_t)1 + ((seq_t)2 * k)) *
                ((seq_t)3 + ((seq_t)2 * k)));
    }
}
