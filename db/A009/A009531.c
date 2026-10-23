/*
 * A009531  Expansion of the e.g.f. sin(x)*(1+x).
 * https://oeis.org/A009531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = { 0 , n = 0 * n , n mod 4 = 2 * -n , n mod 4 = 0 * 1 , n
 *            mod 4 = 1 * -1 , n mod 4 = 3 }
 * Range:     i8 n <= 129, i16 n <= 9655, i32 n <= 9655, i64 n <= 9655,
 *            i128 n <= 9655, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t nval = (seq_t)n;
    switch (n & 3u) {
        case 0u: return -nval;   /* n % 4 == 0  -> -n */
        case 2u: return nval;    /* n % 4 == 2  ->  n */
        case 1u: return (seq_t)1;/* n % 4 == 1  ->  1 */
        default: return (seq_t)-1;/* n % 4 == 3  -> -1 */
    }
}
