/*
 * A035879  Coordination sequence for diamond structure D^+_6. (Edges defined
 *          by l_1 norm = 1.)
 * https://oeis.org/A035879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0; for n > 0, a(n) =
 *            2*n*((2*n^4+20*n^2+23)*(-1)^n + (4*n^4+10*n^2+31))/15
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 89, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;          /* n^2 */
    seq_t n4 = n2 * n2;          /* n^4 */

    seq_t A = 2 * n4 + 20 * n2 + 23;   /* 2*n^4 + 20*n^2 + 23 */
    seq_t B = 4 * n4 + 10 * n2 + 31;   /* 4*n^4 + 10*n^2 + 31 */

    seq_t expr;
    if ((n & 1) == 0) {            /* even n */
        expr = A + B;
    } else {                       /* odd n */
        expr = B - A;              /* B - A is non‑negative for odd n */
    }

    seq_t t = sn << 1;             /* 2*n */

    /* cancel factors 3 and 5 before the final multiplication.
       This keeps the intermediate product within range for all n
       whose final value fits in seq_t. */
    if (t % 3 == 0) {
        t /= 3;
    } else {
        expr /= 3;
    }
    if (t % 5 == 0) {
        t /= 5;
    } else {
        expr /= 5;
    }

    return t * expr;               /* result is 2*n*expr/15 */
}
