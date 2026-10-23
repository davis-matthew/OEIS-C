/*
 * A132356  a(2*k) = k*(10*k+2), a(2*k+1) = 10*k^2 + 18*k + 8, with k >= 0.
 * https://oeis.org/A132356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*k) = k*(10*k+2), k ≥ 0 Formula: a(2*k+1) = 10*k^2 + 18*k +
 *            8, k ≥ 0
 * Range:     u8 n <= 9, u16 n <= 161, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {              /* even index */
        idx_t k = n >> 1;            /* k = n/2   */
        seq_t k_seq = (seq_t)k;
        return k_seq * (10 * k_seq + 2);
    } else {                          /* odd index */
        idx_t k = (n - 1) >> 1;      /* k = (n-1)/2 */
        seq_t k_seq = (seq_t)k;
        return 10 * k_seq * k_seq + 18 * k_seq + 8;
    }
}
