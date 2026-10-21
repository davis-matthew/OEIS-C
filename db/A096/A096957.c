/*
 * A096957  Fourth column (m=3) of (1,6)-Pascal triangle A096956.
 * https://oeis.org/A096957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+18)*(n+2)*(n+1)/6
 * Range:     u8 n <= 6, u16 n <= 66, u32 n <= 2946, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation avoids overflow by cancelling factors of 2 and 3 early. */
seq_t a(idx_t n)
{
    seq_t a = (seq_t)(n + 1);
    seq_t b = (seq_t)(n + 2);
    seq_t c = (seq_t)(n + 18);

    /* cancel factor 2 */
    if ((b & 1) == 0)
        b /= 2;
    else
        a /= 2;

    /* cancel factor 3 */
    if ((c % 3) == 0)
        c /= 3;
    else if ((a % 3) == 0)
        a /= 3;
    else
        b /= 3;   /* guaranteed to be divisible by 3 */

    /* final multiplication */
    seq_t res = a * b;
    res *= c;
    return res;
}
