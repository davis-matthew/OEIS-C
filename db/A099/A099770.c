/*
 * A099770  Expansion of 1/((1-x)*(1-x^2)*(1-x^4)*(1-x^6)).
 * https://oeis.org/A099770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 259, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A099770
 *  Formula: a(n) = floor( (2 n^3 + 39 n^2 + 225 n + 576 
 *                            + 3*(-1)^n n (n+13) ) / 576 )
 *  (generating function 1/((1-x)(1-x^2)(1-x^4)(1-x^6)) )
 */

seq_t a(idx_t n) {
    /* Use a larger intermediate type so the computation never
     * overflows even for the largest n expected in the tests. */
    unsigned __int128 nn = n;
    unsigned __int128 n2 = nn * nn;
    unsigned __int128 n3 = n2 * nn;

    unsigned __int128 term1 = 2 * n3            // 2 n^3
                          + 39 * n2            // + 39 n^2
                          + 225 * nn           // + 225 n
                          + 576;               // + 576

    unsigned __int128 B   = nn * (nn + 13);     // n (n+13)
    unsigned __int128 mul3B = 3 * B;            // 3*n*(n+13)

    unsigned __int128 S;
    if ((n & 1) == 0)                // n even
        S = term1 + mul3B;
    else                             // n odd
        S = term1 - mul3B;           // term1 >= mul3B for all n

    return (seq_t)(S / 576);
}
