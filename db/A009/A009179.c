/*
 * A009179  E.g.f. cosh(x)/(1+x).
 * https://oeis.org/A009179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1,  a(n) = (1+(-1)^n)/2 - n*a(n-1)  for n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t prev = 1;                 /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t base = ((i & 1) == 0) ? (seq_t)1 : (seq_t)0; /* (1+(-1)^i)/2 */
        seq_t cur = base - (seq_t)i * prev;                /* recurrence */
        prev = cur;
    }
    return prev;                    /* a(n) */
}
