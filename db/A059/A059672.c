/*
 * A059672  Sum of binary numbers with n 1's and one (possibly leading) 0.
 * https://oeis.org/A059672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(2^(n+1)-1)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;

    /* compute 2^(n+1) without undefined left‑shift */
    seq_t pow;
    const unsigned int bits = sizeof(seq_t) * 8;
    if (n + 1 < bits) {
        /* safe shift for all integer sizes that fit in <bits> bits */
        pow = ((seq_t)1) << (n + 1);
    } else {
        /* fallback: multiply by 2 repeatedly (unsigned overflow is defined) */
        pow = 1;
        for (idx_t i = 0; i < n + 1; ++i) {
            pow *= 2;
        }
    }

    /* a(n) = n * ( 2^(n+1) - 1 ) */
    seq_t result = n_seq * (pow - 1);
    return result;
}
