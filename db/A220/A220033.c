/*
 * A220033  Number of 3 X n arrays of the minimum value of corresponding
 *          elements and their horizontal or diagonal neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..1 3 X n array.
 * https://oeis.org/A220033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+13)/2 - 15 for n > 3, a(1)=4, a(2)=6, a(3)=10
 * Range:     u8 n <= 10, u16 n <= 210, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)10;

    /* For n > 3 use the closed‑form quadratic expression */
    seq_t nn = (seq_t)n;
    seq_t temp = nn * (nn + 13);      /* n*(n+13) */
    seq_t result = temp / 2;          /* divide by 2 */
    result -= 15;                     /* subtract 15 */
    return result;
}
