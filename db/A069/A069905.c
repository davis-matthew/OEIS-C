/*
 * A069905  Number of partitions of n into 3 positive parts.
 * https://oeis.org/A069905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = floor((n^2 + 6)/12)
 * Range:     u8 n <= 15, u16 n <= 455, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into 3 positive parts.
   The function works for all non-negative n and uses only integer
   arithmetic on the supplied types idx_t and seq_t. */
seq_t a(idx_t n)
{
    /* Convert index to sequence type to avoid intermediate overflow */
    seq_t nn = (seq_t)n;
    seq_t nsq = nn * nn;      /* n^2 */
    seq_t res = (nsq + 6) / 12;  /* floor((n^2 + 6)/12) */
    return res;
}
