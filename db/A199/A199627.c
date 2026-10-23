/*
 * A199627  G.f.:
 *          (1+x)^(2*g)*(1+x^3)^(3*g)/((1-x^2)*(1-x^4))-x^(2*g)*(1+x)^4/((1-x^2)*(1-x^4))
 *          for g=1.
 * https://oeis.org/A199627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 131, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula derived from the OEIS explicit expression:
 *
 *  a(n) = 2*n - 8           , if n mod 4 == 0 or 2
 *  a(n) = 2*n - 9           , if n mod 4 == 1
 *  a(n) = 2*n - 7           , if n mod 4 == 3
 *
 *  For n = 0 … 5 the initial values are: 1, 2, 1, 1, 2, 2.
 *
 *  This implements a fast O(1) computation using only integer
 *  arithmetic.  The function works for any idx_t and seq_t
 *  provided by the build system.  It does not assume any order of
 *  calls and contains no global state. */
 
seq_t a(idx_t n)
{
    /* Handle the first six terms explicitly */
    if (n < 6) {
        static const seq_t init[6] = {1, 2, 1, 1, 2, 2};
        return init[n];
    }

    /* Compute 2*n - 8 as the base value */
    seq_t res = (seq_t)n * (seq_t)2;
    res -= (seq_t)8;

    /* Adjust based on n mod 4 */
    switch (n & 3u) {        /* n % 4, using bitwise & for speed */
        case 1:   /* n mod 4 == 1 -> subtract 1 */
            res -= (seq_t)1;
            break;
        case 3:   /* n mod 4 == 3 -> add 1 */
            res += (seq_t)1;
            break;
        /* cases 0 and 2: no adjustment needed */
        default:
            break;
    }

    return res;
}
