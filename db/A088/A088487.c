/*
 * A088487  a(n) = Sum_{k=1..8} floor(A254864(n,k)/A254864(n-1,k)), where
 *          A254864(n,k) = n! / (n-floor(n/3^k))!.
 * https://oeis.org/A088487
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = Σ_{k=1..8} floor(A254864(n,k)/A254864(n-1,k)), where
 *            A254864(n,k) = n!/(n-floor(n/3^k))! . The ratio simplifies to:
 *            if n mod 3^k == 0 → value = n else → value = floor( n / (n -
 *            floor(n/3^k)) )
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t t = 3;               /* t = 3^k */
    for (int k = 1; k <= 8; ++k) {
        if (n % t == 0) {                  /* n divisible by 3^k */
            sum += (seq_t)n;
        } else {
            idx_t s = n / t;                /* floor(n / 3^k) */
            seq_t denom = (seq_t)(n - s);    /* n - floor(n/3^k)  */
            sum += (seq_t)n / denom;        /* floor division   */
        }
        t *= 3;                           /* next power of 3   */
    }
    return sum;
}
