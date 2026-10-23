/*
 * A168538  a(n) = (n+6)*(n+1)*(n^2 + 7*n + 16)/4.
 * https://oeis.org/A168538
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+6)*(n+1)*(n^2 + 7*n + 16)/4
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 358, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use signed arithmetic for the intermediate calculations. */
    idx_t N = n;
    idx_t part1 = (N + 6) * (N + 1);          /* (n+6)*(n+1)   */
    idx_t part2 = N * N + 7 * N + 16;         /* n^2 + 7*n + 16 */

    /* The product part1*part2 is always even; split the division by 4
       into two sequential divisions by 2 to avoid overflow. */
    idx_t tmp = part1 >> 1;                   /* part1 / 2       */
    idx_t res = tmp * part2;                  /* (part1/2)*part2 */
    res >>= 1;                                /* final / 2       */

    return (seq_t)res;
}
