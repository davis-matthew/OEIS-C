/*
 * A166153  a(n) = (7^n+14*(-8)^n)/15.
 * https://oeis.org/A166153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 10, i64 n <= 21, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=-7, a(n)=56*a(n-2)-a(n-1) */
seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)1;
    }
    if (n == 1u) {
        return (seq_t)-7;
    }
    seq_t a0 = (seq_t)1;    /* a(0) */
    seq_t a1 = (seq_t)-7;   /* a(1) */
    seq_t ai;
    for (idx_t i = 2u; i <= n; ++i) {
        ai = (seq_t)56 * a0 - a1;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
