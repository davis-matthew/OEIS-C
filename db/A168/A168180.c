/*
 * A168180  a(n) = n^3*(n^5 + 1)/2.
 * https://oeis.org/A168180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3*(n^5 + 1)/2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 17, u64 n <= 279, u128 n <= 440,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t sn2 = sn * sn;          /* n^2 */
    seq_t sn3 = sn2 * sn;         /* n^3 */
    seq_t sn5 = sn3 * sn2;        /* n^5 */
    seq_t result;
    if (sn & (seq_t)1) {                      /* n odd: n^5+1 is even */
        seq_t half = (sn5 + (seq_t)1) >> 1;   /* (n^5 + 1)/2 */
        result = sn3 * half;                  /* n^3 * ((n^5+1)/2) */
    } else {                                  /* n even: n^3 is even */
        seq_t sn3half = sn3 >> 1;              /* n^3/2 */
        result = sn3half * (sn5 + (seq_t)1);   /* (n^3/2)*(n^5+1) */
    }
    return result;
}
