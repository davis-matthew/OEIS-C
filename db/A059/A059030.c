/*
 * A059030  Fourth main diagonal of A059026: a(n) = B(n+3,n) =
 *          lcm(n+3,n)/(n+3) + lcm(n+3,n)/n - 1 for all n >= 1.
 * https://oeis.org/A059030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + 3)/gcd(n,3) - 1
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here gcd(n,3) is either 1 or 3 depending on n mod 3. */

seq_t a(idx_t n)
{
    /* Compute g = gcd(n, 3).  Since 3 is prime, gcd is 3 iff n is a multiple of 3. */
    seq_t g = (n % 3 == 0) ? 3 : 1;

    /* a(n) = (2*n + 3)/g - 1  */
    return ((seq_t)2 * n + 3) / g - 1;
}
