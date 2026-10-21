/*
 * A077934  Duplicate of A077973.
 * https://oeis.org/A077934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) is defined by the recurrences * f(0) = 1, g(0) = 1 * f(m+1)
 *            = 4*f(m) + (-1)^(m+1) * g(m+1) = 4*g(m) + (-1)^m * and * a(4m) =
 *            f(m) * a(4m+1) = g(m) * a(4m+2) = f(m) * a(4m+3) = f(m)
 * Range:     i8 n <= 15, i16 n <= 23, i32 n <= 23, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t f = 1;  /* a(4m)  */
    seq_t g = 1;  /* a(4m+1) */
    idx_t m = n / 4;
    for (idx_t i = 1; i <= m; ++i)
    {
        /* (-1)^i : -1 if i odd, +1 if i even */
        seq_t new_f = (seq_t)4 * f + ((i & 1) ? (seq_t)-1 : (seq_t)1);
        /* (-1)^(i-1) : +1 if i odd, -1 if i even */
        seq_t new_g = (seq_t)4 * g + ((i & 1) ? (seq_t)1 : (seq_t)-1);
        f = new_f;
        g = new_g;
    }

    switch (n & 3)
    {
        case 0: return f;  /* a(4m)   */
        case 1: return g;  /* a(4m+1) */
        case 2: return f;  /* a(4m+2) */
        case 3: return f;  /* a(4m+3) */
        default: return 0; /* unreachable */
    }
}
