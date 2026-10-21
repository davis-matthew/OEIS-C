/*
 * A218135  Norm of coefficients in the expansion of 1 / (1 - x - 2*I*x^2),
 *          where I^2=-1.
 * https://oeis.org/A218135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1, a(1) = 1, a(2) = 5, a(3) = 17
   a(n) = a(n-1) + 8*a(n-2) + 4*a(n-3) - 16*a(n-4)   (n >= 4)
*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)17;

    seq_t prev[4] = {(seq_t)1, (seq_t)1, (seq_t)5, (seq_t)17};
    for (idx_t i = 4; i <= n; ++i) {
        seq_t sum = prev[3]
                  + (seq_t)8 * prev[2]
                  + (seq_t)4 * prev[1];
        seq_t next = sum - (seq_t)16 * prev[0];

        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = next;
    }
    return prev[3];
}
