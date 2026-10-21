/*
 * A070641  a(n) = n^6 mod 18.
 * https://oeis.org/A070641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 18
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n mod 18.  The result fits into an unsigned int. */
    unsigned int r = (unsigned int)(n % 18);
    unsigned int res  = 1;          /* running result (<18)      */
    unsigned int base = r;          /* base of exponentiation    */
    unsigned int exp  = 6;          /* exponent n^6              */

    /* Binary exponentiation, keeping values <18 to avoid overflow. */
    while (exp) {
        if (exp & 1u)
            res = (res * base) % 18;
        base = (base * base) % 18;
        exp >>= 1u;
    }
    return (seq_t)res; /* result is already <18 */
}
