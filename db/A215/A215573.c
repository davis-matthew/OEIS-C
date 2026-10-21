/*
 * A215573  a(n) = n*(n+1)*(2n+1)/6 modulo n.
 * https://oeis.org/A215573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000330(n) mod n = n*(n+1)*(2n+1)/6 mod n
 * Range:     u8 n <= 386, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         Piecewise definition:                         */
/*           a(6k)   = k                        */
/*           a(6k+1) = 0                        */
/*           a(6k+2) = 3k+1                    */
/*           a(6k+3) = 4k+2                    */
/*           a(6k+4) = 3k+2                    */
/*           a(6k+5) = 0                        */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n = 0, return 0 */

    idx_t k = n / 6;
    idx_t r = n % 6;
    idx_t res = 0;          /* fits into idx_t and hence into seq_t */

    switch (r) {
        case 0:            /* n = 6k     */
            res = k;
            break;
        case 1:            /* n = 6k+1   */
            res = 0;
            break;
        case 2:            /* n = 6k+2   */
            res = 3 * k + 1;
            break;
        case 3:            /* n = 6k+3   */
            res = 4 * k + 2;
            break;
        case 4:            /* n = 6k+4   */
            res = 3 * k + 2;
            break;
        case 5:            /* n = 6k+5   */
            res = 0;
            break;
    }
    return (seq_t)res;
}
