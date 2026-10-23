/*
 * A011894  a(n) = floor(n*(n-1)*(n-2)/12).
 * https://oeis.org/A011894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)/12)
 * Range:     u8 n <= 9, u16 n <= 92, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n < 3 => C(n,3) = 0, so a(n) = 0 */
    if (n < 3)
        return 0;

    /* Compute C(n,3) = n*(n-1)*(n-2)/6 using a loop that divides at
       each step to keep the intermediate value small. */
    seq_t res = 1;
    for (idx_t i = 1; i <= 3; ++i) {
        seq_t factor = (seq_t)(n - 3 + i);   /* n-2, n-1, n */
        res *= factor;
        res /= (seq_t)i;                     /* exact division */
    }

    /* a(n) = floor(C(n,3)/2) */
    return res / 2;
}
