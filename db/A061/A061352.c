/*
 * A061352  First row of array shown below.
 * https://oeis.org/A061352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + ((n-1+1)/2) + 2*((n-1)/2)*((n-1)/2+1)) mod 10
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  For the OEIS offset, n starts at 1.  If n==0 the function
        returns 0, but 0 is never part of the sequence.          */
    if (n == 0)
        return 0;

    /*  m = n-1  (the number of steps already performed)               */
    idx_t m = n - 1;

    /*  Count of even and odd indices among 1 … m                       */
    idx_t evenCount = m / 2;            /*  even indices: 2,4,…          */
    idx_t oddCount  = (m + 1) / 2;      /*  odd  indices: 1,3,…          */

    /*  Compute the even‑contribution modulo 10 safely                */
    seq_t even_mod = (seq_t)(evenCount % 10);           /* <10 */
    seq_t next_mod = (seq_t)((evenCount + 1) % 10);     /* next integer modulo 10 */
    seq_t tmp = (seq_t)(( (even_mod * next_mod) % 10 ) * 2 % 10);

    /*  Combine with the odd contribution and the initial value      */
    idx_t odd_mod = oddCount % 10;
    seq_t result = (seq_t)(( odd_mod + tmp + 1 ) % 10);

    return result;
}
