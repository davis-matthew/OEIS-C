/*
 * A094648  An accelerator sequence for Catalan's constant.
 * https://oeis.org/A094648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 17, i32 n <= 36, i64 n <= 74, i128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+3) + a(n+2) - 2*a(n+1) - a(n) = 0,
   a(0) = 3, a(1) = -1, a(2) = 5 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)5;

    seq_t a0 = (seq_t)3;   /* a(n-3)  */
    seq_t a1 = (seq_t)-1;  /* a(n-2)  */
    seq_t a2 = (seq_t)5;   /* a(n-1)  */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = a0 + (seq_t)2 * a1 - a2; /* a(n) = -a(n-1) + 2*a(n-2) + a(n-3) */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
