/*
 * A094625  Expansion of g.f. x*(2+22*x+11*x^2)/((x-1)*(1+x)*(10*x^2-1)).
 * https://oeis.org/A094625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) = Expansion of g.f. x*(2+22*x+11*x^2)/((x-1)*(1+x)*(10*x^2-1))
   via digit pattern:
   a(0)=0, a(1)=2.
   For n≥2:
     if n is even: a(n) = 2 followed by (n/2−1) copies of '4' and a final 2
     if n is odd : a(n) = 3 followed by ((n−3)/2) copies of '4' and a final 3
   The number is built digit by digit using only integer arithmetic.
*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* a(0) = 0 */
    if (n == 1) return 2;            /* a(1) = 2 */
    seq_t val = 0;

    if ((n & 1) == 0) {              /* even n = 2k */
        val = 2;                     /* first digit */
        idx_t len4 = n / 2 - 1;      /* number of '4' digits */
        for (idx_t i = 0; i < len4; ++i)
            val = val * 10 + 4;
        val = val * 10 + 2;          /* final digit */
    } else {                         /* odd n = 2k+1, k≥1 */
        val = 3;                     /* first digit */
        idx_t len4 = (n - 3) / 2;    /* number of '4' digits */
        for (idx_t i = 0; i < len4; ++i)
            val = val * 10 + 4;
        val = val * 10 + 3;          /* final digit */
    }
    return val;
}
