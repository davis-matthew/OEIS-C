/*
 * A139798  Coefficient of x^5 in (1-x-x^2)^(-n).
 * https://oeis.org/A139798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n^2+27*n+132)/120
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a0 = (seq_t)n;          /* n */
    seq_t a1 = a0 + 1;            /* n+1 */
    seq_t a2 = a0 + 2;            /* n+2 */
    seq_t a3 = a0 * a0 + a0 * 27 + 132; /* n^2+27*n+132 */

    /* Divide by prime factors of 120 = 2^3 * 3 * 5.
       Each factor surely divides at least one of the four terms. */
    if (a0 % 2 == 0) a0 /= 2; else if (a1 % 2 == 0) a1 /= 2; else if (a2 % 2 == 0) a2 /= 2; else a3 /= 2;
    if (a0 % 2 == 0) a0 /= 2; else if (a1 % 2 == 0) a1 /= 2; else if (a2 % 2 == 0) a2 /= 2; else a3 /= 2;
    if (a0 % 2 == 0) a0 /= 2; else if (a1 % 2 == 0) a1 /= 2; else if (a2 % 2 == 0) a2 /= 2; else a3 /= 2;
    if (a0 % 3 == 0) a0 /= 3; else if (a1 % 3 == 0) a1 /= 3; else if (a2 % 3 == 0) a2 /= 3; else a3 /= 3;
    if (a0 % 5 == 0) a0 /= 5; else if (a1 % 5 == 0) a1 /= 5; else if (a2 % 5 == 0) a2 /= 5; else a3 /= 5;

    return a0 * a1 * a2 * a3;
}
