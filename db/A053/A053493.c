/*
 * A053493  Number of symmetric 4 X 4 matrices of nonnegative integers with
 *          every row and column adding to n.
 * https://oeis.org/A053493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*(31+(-1)^n) + 768*n + 928*n^2 + 624*n^3 + 238*n^4 +
 *            48*n^5 + 4*n^6) / 288
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 80, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit intermediate arithmetic to avoid overflow for large n. */
    unsigned __int128 nn = n;
    unsigned __int128 n2 = nn * nn;
    unsigned __int128 n3 = n2 * nn;
    unsigned __int128 n4 = n3 * nn;
    unsigned __int128 n5 = n4 * nn;
    unsigned __int128 n6 = n5 * nn;

    /* 31 + (-1)^n   -> 32 for even n, 30 for odd n */
    unsigned __int128 term0 = (n & 1U) ? 30U : 32U;

    unsigned __int128 num = term0 * 9U;
    num += 768U * nn;
    num += 928U * n2;
    num += 624U * n3;
    num += 238U * n4;
    num += 48U * n5;
    num += 4U  * n6;

    seq_t result = (seq_t)(num / 288U);
    return result;
}
