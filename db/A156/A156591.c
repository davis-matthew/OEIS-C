/*
 * A156591  First differences of A154570.
 * https://oeis.org/A156591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 2; for n > 0, a(n) = (-2^n + 40*(-1)^n)/6. Recurrence
 *            used in the implementation: a(1) = -7, and a(k+1) = -2^(k-1) -
 *            a(k) for k ≥ 1. This is equivalent to the above closed form. The
 *            routine works for any seq_t (signed integer or
 *            arbitrary‑precision) and for all n ≥ 0
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 33, i64 n <= 65, i128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)-7;

    seq_t power = (seq_t)1;    /* 2^0 */
    seq_t curr  = (seq_t)-7;   /* a(1)  */
    idx_t  i;

    for (i = 1; i < n; ++i) {  /* compute a(2) … a(n) */
        seq_t next  = -power - curr;  /* a(i+1) = -2^(i-1) - a(i) */
        power = power * (seq_t)2;     /* next power of two */
        curr  = next;
    }
    return curr;
}
