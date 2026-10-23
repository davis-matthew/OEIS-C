/*
 * A020623  Smallest nonempty set S containing prime divisors of 9k+1 for each
 *          k in S.
 * https://oeis.org/A020623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A020623: primes in the minimal nonempty set closed under
   “prime divisors of 9k+1 for each k in the set”.  Values are stored
   in a static table because the set is finite. */
seq_t a(idx_t n)
{
    static const seq_t table[] = {2, 7, 13, 19, 23, 43, 59, 97};
    if (n >= 1 && n <= sizeof(table) / sizeof(table[0]))
        return table[n - 1];
    return 0;
}
