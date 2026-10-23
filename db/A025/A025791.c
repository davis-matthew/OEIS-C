/*
 * A025791  Expansion of 1/((1-x)*(1-x^9)*(1-x^10)).
 * https://oeis.org/A025791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 20*n + 10*(2+(n mod 9))*(9-(n mod 9)) - 9*(n mod
 *            10)*(10-(n mod 10)))/180
 * Range:     u8 n <= 8, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts 1, 9, and 10. */
seq_t a(idx_t n)
{
    /* Cast the index to the value type to perform arithmetic safely. */
    seq_t nn = (seq_t)n;

    /* Term: n^2 + 20*n */
    seq_t term1 = nn * nn + (seq_t)(20 * nn);

    /* Compute modulo 9 and 10 values as idx_t to keep multiplications limited. */
    idx_t mod9  = n % 9;
    idx_t mod10 = n % 10;

    /* Term: 10 * (2 + mod9) * (9 - mod9) */
    idx_t tmp1 = (2 + mod9) * (9 - mod9);          /* 0 <= tmp1 <= 90 */
    seq_t term2 = (seq_t)(10 * tmp1);

    /* Term: 9 * mod10 * (10 - mod10) */
    idx_t tmp2 = mod10 * (10 - mod10);            /* 0 <= tmp2 <= 45 */
    seq_t term3 = (seq_t)(9 * tmp2);

    /* Combine terms and divide by 180. */
    seq_t numerator = term1 + term2 - term3;
    return numerator / (seq_t)180;
}
