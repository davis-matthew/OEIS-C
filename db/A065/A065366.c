/*
 * A065366  Replace 3^k with (-2)^k in balanced ternary expansion of n.
 * https://oeis.org/A065366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(n) = Σ d_k · (−2)^k where d_k ∈ {−1,0,1} are the balanced
 *            ternary digits of n (least‑significant digit has k=0)
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow = 1;                 /* current power of (−2)^k   */
    idx_t m = n;                   /* working variable           */

    while (m != 0) {
        int r = (int)(m % 3);     /* balanced ternary remainder */
        m /= 3;

        int digit;
        if (r == 0) {
            digit = 0;
        } else if (r == 1) {
            digit = 1;
        } else {                  /* r == 2 → use digit −1 */
            digit = -1;
            m += 1;               /* carry over */
        }

        result += (seq_t)digit * pow;
        pow *= (seq_t)(-2);
    }
    return result;
}
