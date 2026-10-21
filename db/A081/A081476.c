/*
 * A081476  Consider the mapping f(x/y) = (x+y)/(2xy) where x/y is a reduced
 *          fraction. Beginning with x_0 = 1 and y_0 = 2, repeated application
 *          of this mapping produces a sequence of fractions x_n/y_n; a(n) is
 *          the n-th denominator.
 * https://oeis.org/A081476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A081476:  Denominators of the iterates of f(x/y) = (x+y)/(2xy)
   with reduced fraction, starting from 1/2.
   We maintain the pair (x_n , y_n) with
     x_{n+1} = (x_n + y_n) / g,          (g = 2 if x_n and y_n are odd, else 1)
     y_{n+1} = (2 * x_n * y_n) / g.
   The denominator a(n) is y_n.  This is computed iteratively in O(n) time.
*/

seq_t a(idx_t n)
{
    /* Return a(0) directly */
    if (n == 0)
        return (seq_t)2;

    seq_t x = 1;   /* numerator */
    seq_t y = 2;   /* denominator = a(0) */

    /* Iterate from 1 up to n */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t sum = x + y;
        seq_t new_x, new_y;

        if ((x & (seq_t)1) && (y & (seq_t)1)) {
            /* gcd = 2, divide numerator and denominator by 2 */
            new_x = sum / (seq_t)2;
            new_y = x * y;         /* y_{n+1} = x_n * y_n */
        } else {
            new_x = sum;           /* keep full sum */
            new_y = (x * y) * (seq_t)2;
        }

        x = new_x;
        y = new_y;
    }

    return y;
}
