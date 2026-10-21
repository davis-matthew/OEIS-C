/*
 * A057525  Number of applications of f to reduce n to 1, where f(k) is the
 *          integer among k/2,(k+1)/4, (k+3)/4.
 * https://oeis.org/A057525
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of times one must apply the function f to reduce n
 *            to 1, where f(k) is the unique integer among k/2, (k+1)/4,
 *            (k+3)/4. (For even k, f(k)=k/2; for k≡3 mod4, f(k)=(k+1)/4; for
 *            k≡1 mod4, f(k)=(k+3)/4.)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2)
        return (seq_t)0;
    idx_t steps = 0;
    while (n != 1) {
        if ((n & 1) == 0) {                 /* even case */
            n >>= 1;                        /* n = n/2 */
        } else {                            /* odd case */
            /* n is odd, so n-1 or n-3 is divisible by 4 */
            if ((n & 3) == 3) {             /* n ≡ 3 (mod 4) -> (n+1)/4 */
                n = ((n - 3) >> 2) + 1;
            } else {                         /* n ≡ 1 (mod 4) -> (n+3)/4 */
                n = ((n - 1) >> 2) + 1;
            }
        }
        ++steps;
    }
    return (seq_t)steps;
}
