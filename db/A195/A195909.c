/*
 * A195909  First numerator and then denominator in a fraction expansion of
 *          log(2) - Pi/8.
 * https://oeis.org/A195909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, sign
 * Formula:   not stated
 * Range:     i8 n <= 17, i16 n <= 53, i32 n <= 53, i64 n <= 53, i128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A195909: Numerators and denominators of the fraction expansion
   of log(2) - Pi/8. For odd n the term is ±1 (the numerator);
   for even n it is the denominator of the corresponding fraction.

   Let i = ceil(n/2) be the pair index (1‑based). Then

      sign  = +1  if i%3 != 2
      sign  = -1  if i%3 == 2

   Denominator:
      let c = (i-1)/3   and   r = (i-1)%3

      if r==0:  j = 4c+1;  denom = j*(j+1);
      if r==1:  j = 4c+1;  denom = j*(j+2);
      if r==2:  j = 4c+3;  denom = j*(j+1);
*/

seq_t a(idx_t n)
{
    /* pair index i = ceil(n/2) */
    idx_t pair = (n + 1) / 2;

    if ((n & 1) != 0) {   /* odd n – numerator */
        return ((pair % 3) == 2) ? (seq_t)(-1) : (seq_t)(1);
    }

    /* even n – denominator */
    idx_t c   = (pair - 1) / 3;
    idx_t r   = (pair - 1) % 3;
    seq_t j, denom;

    if (r == 0) {                 /* j = 4c+1  →  j*(j+1) */
        j = (seq_t)(4 * c + 1);
        denom = j * (j + (seq_t)1);
    } else if (r == 1) {          /* j = 4c+1  →  j*(j+2) */
        j = (seq_t)(4 * c + 1);
        denom = j * (j + (seq_t)2);
    } else { /* r == 2 */          /* j = 4c+3  →  j*(j+1) */
        j = (seq_t)(4 * c + 3);
        denom = j * (j + (seq_t)1);
    }

    return denom;
}
