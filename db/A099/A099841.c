/*
 * A099841  Expansion of (1-16*x)/(1-20*x+80*x^2).
 * https://oeis.org/A099841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=4, a(n)=20*a(n-1)-80*a(n-2)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 17, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   recurrence rewritten as  a(n)=4*(5*a(n-1)-20*a(n-2)) 
 *   to reduce intermediate overflow for 64‑bit builds. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 4;

    seq_t a0 = 1;     /* a(n-2) */
    seq_t a1 = 4;     /* a(n-1) */
    seq_t a2;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t t   = (seq_t)5 * a1 - (seq_t)20 * a0;
        a2 = (seq_t)4 * t;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
