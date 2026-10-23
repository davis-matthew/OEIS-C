/*
 * A165422  a(1) = 1, a(2) = 4, a(n) = product of the previous terms for n >=
 *          3.
 * https://oeis.org/A165422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=4, a(3)=4, a(n)=a(n-1)^2 for n>=4 */
seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 1) {
        return (seq_t)1;
    }
    if (n == 2 || n == 3) {
        return (seq_t)4;
    }

    /* For n >= 4 compute iteratively: a(n) = a(n-1)^2 */
    seq_t val = (seq_t)4;              /* a(3) */
    for (idx_t i = 4; i <= n; ++i) {   /* compute a(4), a(5), …, a(n) */
        val = val * val;               /* a(i) = a(i-1)^2 */
    }
    return val;
}
