/*
 * A116766  Number of permutations of length n which avoid the patterns 2134,
 *          3421, 4312.
 * https://oeis.org/A116766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n⁴ - 5·n³ + 9·n² - 57·n + 202)/2 (for n ≥ 5) Initial
 *            terms: a(1)=1, a(2)=2, a(3)=6, a(4)=21
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return fixed values for the first four indices */
    switch (n)
    {
        case 1:  return (seq_t)1;
        case 2:  return (seq_t)2;
        case 3:  return (seq_t)6;
        case 4:  return (seq_t)21;
        default:
            break;
    }

    /* For n ≥ 5 compute the polynomial in a single expression
       using only integer arithmetic.
       Using seq_t for all intermediate values guarantees exactness
       for any supported unsigned integer or arbitrary-precision type. */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;        /* n² */
    seq_t n3 = n2 * nn;        /* n³ */
    seq_t n4 = n3 * nn;        /* n⁴ */

    seq_t term = n4
               - (seq_t)5 * n3
               + (seq_t)9 * n2
               - (seq_t)57 * nn
               + (seq_t)202;

    return term / (seq_t)2;
}
