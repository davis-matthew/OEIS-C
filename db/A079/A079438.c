/*
 * A079438  a(0) = a(1) = 1, a(n) = 2*(floor((n+1)/3) + (if n >= 14)
 *          (floor((n-10)/4) + floor((n-14)/8))).
 * https://oeis.org/A079438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 1, a(n) = 2 * ( floor((n+1) / 3) + (n >= 14 ?
 *            floor((n-10) / 4) + floor((n-14) / 8) : 0) )
 * Range:     u8 n <= 187, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes term a(n) of OEIS sequence A079438 */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t sn = (seq_t)n;            /* cast the index to seq_t */
    seq_t val = (sn + (seq_t)1) / (seq_t)3;  /* floor((n+1)/3) */

    if (n >= 14) {
        val += (sn - (seq_t)10) / (seq_t)4;   /* floor((n-10)/4) */
        val += (sn - (seq_t)14) / (seq_t)8;   /* floor((n-14)/8) */
    }

    return val * (seq_t)2;                  /* final result */
}
