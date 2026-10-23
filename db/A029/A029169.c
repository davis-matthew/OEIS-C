/*
 * A029169  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^10)*(1-x^12)).
 * https://oeis.org/A029169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+45*n^2+684*n+5184)/4320 * - (n^2+27*n+186)*(n
 *            mod 2)/480 * - (n+8)*(n^2 mod 3)/36)
 * Range:     u8 none, u16 n <= 25, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts 2, 3, 10 and 12. */
seq_t a(idx_t n)
{
    /* Convert n to the sequence type to avoid intermediate over‑flows. */
    seq_t nn = (seq_t)n;

    /* t1 = n^3 + 45*n^2 + 684*n + 5184 */
    seq_t t1 = nn * nn * nn
             + (seq_t)45 * nn * nn
             + (seq_t)684 * nn
             + (seq_t)5184;

    /* second term: 9 * (n^2 + 27*n + 186) * (n mod 2)   (common denom 4320) */
    seq_t mod2 = nn & (seq_t)1;                 /* n mod 2 */
    seq_t t2 = (seq_t)9 *
              (nn * nn + (seq_t)27 * nn + (seq_t)186) *
              mod2;

    /* third term: 120 * (n + 8) * (n^2 mod 3)           (common denom 4320) */
    seq_t sqmod3 = (nn * nn) % (seq_t)3;          /* n^2 mod 3 */
    seq_t t3 = (seq_t)120 *
              (nn + (seq_t)8) *
              sqmod3;

    /* Numerator of the combined fraction (denominator = 4320). */
    seq_t N = t1 - t2 - t3;

    /* Integer division implements the floor operation. */
    return N / (seq_t)4320;
}
