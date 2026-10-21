/*
 * A084091  Expansion of Sum_{k>=0} x^2^k/(1+x^2^k+x^2^(k+1)).
 * https://oeis.org/A084091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   not stated
 * Range:     i8 n <= 9967, i16 n <= 9967, i32 n <= 9967, i64 n <= 9967,
 *            i128 n <= 9967, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1;
   a(2n)   = a(n) + 1 - ((n+1) mod 3);
   a(2n+1) = 1 - (n mod 3); */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    if ((n & 1) == 0) {          /* n is even: n = 2*m */
        idx_t m = n >> 1;
        seq_t half = a(m);
        seq_t mod3 = (seq_t)((m + 1) % 3);
        return half + (1 - mod3);
    } else {                     /* n is odd: n = 2*m + 1 */
        idx_t m = n >> 1;
        seq_t mod3 = (seq_t)(m % 3);
        return 1 - mod3;
    }
}
