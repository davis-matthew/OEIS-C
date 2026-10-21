/*
 * A092695  Number of positive integers less than or equal to n which are not
 *          divisible by the primes 2,3,5,7.
 * https://oeis.org/A092695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n – ⌊n/2⌋ – ⌊n/3⌋ – ⌊n/5⌋ – ⌊n/7⌋ + ⌊n/6⌋ + ⌊n/10⌋ +
 *            ⌊n/14⌋ + ⌊n/15⌋ + ⌊n/21⌋ + ⌊n/35⌋ – ⌊n/30⌋ – ⌊n/42⌋ – ⌊n/70⌋ –
 *            ⌊n/105⌋ + ⌊n/210⌋ (inclusion–exclusion for primes 2,3,5,7)
 * Range:     u8 n <= 1116, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a wider type for the intermediate sums to avoid overflow
       when n is large.  The cast to seq_t is safe for all values
       that fit in the target type. */
    __int128 total = (__int128)n;

    total -= ( (__int128)n/2 + (__int128)n/3
             + (__int128)n/5 + (__int128)n/7 );

    total += ( (__int128)n/6 + (__int128)n/10
             + (__int128)n/14 + (__int128)n/15
             + (__int128)n/21 + (__int128)n/35 );

    total -= ( (__int128)n/30 + (__int128)n/42
             + (__int128)n/70 + (__int128)n/105 );

    total += (__int128)n/210;

    return (seq_t)total;
}
