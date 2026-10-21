/*
 * A092093  Back and Forth Summant S(n, _5): a(n) = sum_{i = 0..floor(2n/5)}
 *          n-5i.
 * https://oeis.org/A092093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{i=0..floor(2n/5)} (n - 5i)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 5;                       /* n mod 5  */
    seq_t k = (seq_t)((n - r) / 5);        /* floor(n/5) (= (n-r)/5) */

    switch (r) {
        case 0: return 0;                 /* a(5k) = 0 */
        case 1: return 2 * k + 1;         /* a(5k+1) = 2k+1 */
        case 2: return 4 * k + 2;         /* a(5k+2) = 4k+2 */
        case 3: return k + 1;             /* a(5k+3) = k+1 */
        case 4: return 3 * k + 3;         /* a(5k+4) = 3k+3 */
    }
    return 0; /* unreachable, keeps compiler happy */
}
