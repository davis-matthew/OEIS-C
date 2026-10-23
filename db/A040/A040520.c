/*
 * A040520  Continued fraction for sqrt(544).
 * https://oeis.org/A040520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) = (63 + 51*(-1)^n + 70*cos(n*Pi/2))/4, a(0)=23
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodicity: a(n+4)=a(n) for n>=1 */

seq_t a(idx_t n)
{
    /* Direct handling for the offset */
    if (n == 0)
        return (seq_t)23;

    /* Values repeat every four terms: 46, 3, 11, 3 */
    static const seq_t table[4] = {
        (seq_t)46, /* n mod 4 == 0  (n=4,8,12,...) */
        (seq_t)3,  /* n mod 4 == 1  (n=1,5,9,...) */
        (seq_t)11, /* n mod 4 == 2  (n=2,6,10,...) */
        (seq_t)3   /* n mod 4 == 3  (n=3,7,11,...) */
    };
    return table[n % 4];
}
