/*
 * A103154  Each letter appears an even number of times in the English names
 *          for 1 through n taken together (names without "and").
 * https://oeis.org/A103154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = 119 + 20*((n-1)%5) + 200*((n-1)/5)
 * Range:     u8 n <= 5, u16 n <= 1636, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the periodicity a(n+5) = a(n) + 200 with the first five terms
   119, 139, 159, 179, 199. */
seq_t a(idx_t n)
{
    /* offset n by 1 to work with 0‑based arithmetic */
    idx_t t = n - 1;
    idx_t r = t % 5;  /* remainder 0..4 */
    idx_t q = t / 5;  /* quotient */

    return (seq_t)119 + (seq_t)20 * (seq_t)r + (seq_t)200 * (seq_t)q;
}
