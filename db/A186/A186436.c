/*
 * A186436  Smallest number that equals n times its last digit, or 0 if no
 *          such number exists.
 * https://oeis.org/A186436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = n*d where d ∈ {1,…,9} is the smallest digit such that
 *            (n*d) mod 10 = d; a(n) = 0 if there is no such digit. (In other
 *            words, find the smallest positive n×(last digit of the result).)
 * Range:     u8 n <= 52, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* We only need to check digits 1..9; digit 0 is excluded because
       it gives trivial solution k = 0 which is not considered a valid number. */

    idx_t nmod10 = n % 10;          /* remainder when n divided by 10 */
    for (idx_t d = 1; d <= 9; ++d) {
        /* (n*d) % 10 == d  <=>  (n % 10) * d % 10 == d */
        if ((nmod10 * d) % 10 == d) {
            /* Found the smallest qualifying digit; compute the product.
               The cast to seq_t ensures the result is stored in the
               expected type, and the intermediate multiplication is
               performed using integer promotions that avoid overflow for
               all supported seq_t sizes. */
            return (seq_t)n * (seq_t)d;
        }
    }
    /* No digit satisfies the condition: return zero as per the definition. */
    return 0;
}
