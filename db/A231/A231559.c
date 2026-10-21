/*
 * A231559  a(n) = floor( A000326(n)/2 ).
 * https://oeis.org/A231559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( n*(3*n-1) + i^(n*(n+1)) - 1 )/4
 * Range:     u8 n <= 9, u16 n <= 162, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 to avoid underflow */
    if (n == 0)
        return 0;

    /* Promote n to seq_t for arithmetic */
    seq_t nn = (seq_t)n;

    /* term = n*(3*n-1) */
    seq_t term = nn * ( ((seq_t)3 * nn) - (seq_t)1 );

    /* The i^(n*(n+1)) factor is 1 when n % 4 == 0 or 3,
       and -1 when n % 4 == 1 or 2.  This turns into
       a simple conditional on the binary remainder modulo 4. */
    idx_t mod4 = n & 3;

    if (mod4 == 0 || mod4 == 3) {
        /* i^(...) = 1  -> divide term by 4 */
        return term / 4;
    } else {
        /* i^(...) = -1 -> (term - 2)/4  */
        return (term - (seq_t)2) / 4;
    }
}
