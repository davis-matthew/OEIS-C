/*
 * A168201  Number of representations of n in the form 7*k+11*m (with
 *          nonnegative k, m).
 * https://oeis.org/A168201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative integer solutions to 7k + 11m = n
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;     /* use seq_t for all arithmetic */
    seq_t maxm = sn / 11;      /* maximum possible value of m */
    seq_t mod  = (sn * 2) % 7; /* first m ≡ 2n (mod 7)  (since 4⁻¹ ≡ 2 mod 7) */

    if (mod > maxm)
        return 0;

    return ((maxm - mod) / 7) + 1;
}
