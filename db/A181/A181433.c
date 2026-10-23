/*
 * A181433  Numbers k such that 11*k is 5 less than a square.
 * https://oeis.org/A181433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (22*n^2 - 32*n + 8)/8 if n even (22*n^2 - 12*n - 2)/8 if
 *            n odd (equivalent to a(n) = (22*n*(n-1) - 5*(2*n-1)*(-1)^n +
 *            3)/8)
 * Range:     u8 n <= 10, u16 n <= 154, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) /* sequence is defined for n >= 1; return 0 for safety */
        return 0;

    unsigned __int128 nn = n;          /* to avoid overflow in products   */
    unsigned __int128 n2 = nn * nn;    /* n squared, fits in 128 bits      */

    unsigned __int128 num;
    if (n & 1) {                       /* n is odd                         */
        num = (unsigned __int128)22 * n2
            - (unsigned __int128)12 * nn
            - (unsigned __int128)2;
    } else {                           /* n is even                        */
        num = (unsigned __int128)22 * n2
            - (unsigned __int128)32 * nn
            + (unsigned __int128)8;
    }

    return (seq_t)(num >> 3);          /* divide by 8 (num is always multiple of 8) */
}
