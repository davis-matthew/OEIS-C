/*
 * A014820  a(n) = (1/3)*(n^2 + 2*n + 3)*(n+1)^2.
 * https://oeis.org/A014820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/3)*(n^2 + 2*n + 3)*(n+1)^2
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 335, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: The division by 3 is exact; we perform it after computing
   the factors to keep intermediate values within bounds. */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute the two factors of the numerator */
    seq_t part1 = nn * nn + 2 * nn + 3;   /* n^2 + 2n + 3 */
    seq_t np1   = nn + 1;
    seq_t part2 = np1 * np1;             /* (n+1)^2 */

    /* One of part1 or part2 is guaranteed to be divisible by 3 */
    if (part1 % (seq_t)3 == 0) {
        part1 = part1 / (seq_t)3;
    } else {
        part2 = part2 / (seq_t)3;
    }

    return part1 * part2;
}
