/*
 * A080150  Let m = Wonderful Demlo number A002477(n); a(n) = square of the
 *          sum of digits of m.
 * https://oeis.org/A080150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ( ((n-1)%9 + 1)^2 + 81 * ((n-1)/9) )^2
 * Range:     u8 n <= 3, u16 n <= 30, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Computing the remainders needed for the closed‐form expression. */
    idx_t r = (n - 1) % 9 + 1;   /* r = 1 .. 9 */
    idx_t k = (n - 1) / 9;       /* number of complete 9‑blocks */

    /* Use seq_t for all arithmetic to avoid overflow at the final
       squaring step.                                                      */
    seq_t base = (seq_t)(r * r) + (seq_t)81 * (seq_t)k;
    return base * base;
}
