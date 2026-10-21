/*
 * A226293  Class of sequences of (p-1)-tuples of reverse order of natural
 *          numbers for p = 7.
 * https://oeis.org/A226293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*ceil(n/6) - (1+((n-1) mod 6))
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* offset is 1, behaviour below ignored */
    /* ceil(n/6) calculated safely using idx_t arithmetic */
    idx_t q_idx = n / 6 + ((n % 6) != 0);
    seq_t q = (seq_t)q_idx;
    seq_t b = 1 + (seq_t)((n - 1) % 6);
    return q * 7 - b;
}
