/*
 * A218530  Partial sums of floor(n/11).
 * https://oeis.org/A218530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = sum_{k=0}^n floor(k/11) = 11*q*(q-1)/2 + (r+1)*q, where q
 *            = n/11 and r = n%11
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed non‑negative */
    idx_t q = n / 11;
    idx_t r = n % 11;

    /* Use seq_t for arithmetic to avoid overflow in the target type */
    seq_t q_s = (seq_t)q;
    seq_t term1 = 11 * q_s * (q_s - 1) / 2;
    seq_t term2 = (seq_t)(r + 1) * q_s;

    return term1 + term2;
}
