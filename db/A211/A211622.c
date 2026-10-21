/*
 * A211622  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...-1,1,...,n} and w+2x+3y>1.
 * https://oeis.org/A211622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0 For n > 0: if n is even: a(n) = (8·n³ – 4·n² + 3·n – 2)
 *            / 2 if n is odd: a(n) = (16·n³ – 8·n² + 6·n – 2) / 4
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 688, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t nn   = (seq_t)n;
    seq_t two  = (seq_t)2;
    seq_t four = (seq_t)4;

    if ((n & 1) == 0) {          /* even n */
        seq_t val = ( ( (seq_t)8 * nn * nn * nn )
                    - ( (seq_t)4 * nn * nn )
                    + ( (seq_t)3 * nn )
                    - (seq_t)2 ) / two;
        return val;
    } else {                      /* odd n */
        seq_t val = ( ( (seq_t)16 * nn * nn * nn )
                    - ( (seq_t)8 * nn * nn )
                    + ( (seq_t)6 * nn )
                    - (seq_t)2 ) / four;
        return val;
    }
}
