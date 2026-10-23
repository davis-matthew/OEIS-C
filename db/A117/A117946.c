/*
 * A117946  a(3n)=0, a(3n+1)/a(1)=a(3n+2)/a(2)=A059151(n).
 * https://oeis.org/A117946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 98, i16 n <= 98, i32 n <= 98, i64 n <= 98, i128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117946
 * a(3n)   = 0
 * a(3n+1) = 2^(number of 1‑digits in the base‑3 representation of n)
 * a(3n+2) = -2 * 2^(number of 1‑digits in the base‑3 representation of n)
 *
 * The implementation uses only integer arithmetic and runs in
 * O(log₃ n) time because it scans the base‑3 digits of n.
 */
seq_t a(idx_t n)
{
    idx_t k = n / 3;           /* floor(n/3)  */
    idx_t r = n % 3;           /* n mod 3     */
    if (r == 0)                /* a(3k)       */
        return 0;

    /* compute 2^(count of digits equal to 1 in base‑3 of k) */
    seq_t val = 1;
    while (k) {
        if (k % 3 == 1)
            val <<= 1;        /* multiply by 2 */
        k /= 3;
    }

    if (r == 1)                /* a(3k+1)     */
        return val;
    /* r == 2   a(3k+2) */
    return -(val << 1);        /* -2 * val    */
}
