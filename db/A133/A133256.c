/*
 * A133256  a(4*n+1) = 4*n+1, a(4*n+2) = 4*n+2, a(4*n+3) = 4*n+4, a(4*n+4) =
 *          4*n+3.
 * https://oeis.org/A133256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n mod 4 = 1 or 2 * a(n) = n + 1 if n mod 4 = 3 *
 *            a(n) = n - 1 if n mod 4 = 0
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n=0 in the OEIS offset, return 0 */
        return (seq_t)0;
    }
    seq_t rn = (seq_t)n;     /* promote index to result type */
    switch (n & 3) {         /* n % 4 using bitwise AND (n is uint64_t) */
        case 1:              /* n = 4k+1 */
        case 2:              /* n = 4k+2 */
            return rn;
        case 3:              /* n = 4k+3 */
            return rn + 1;
        case 0:              /* n = 4k   (i.e., 4k+4 in 1‑based notation) */
            return rn - 1;
    }
    /* unreachable: switch covers all cases */
    return rn;
}
