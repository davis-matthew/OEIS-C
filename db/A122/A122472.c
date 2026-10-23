/*
 * A122472  From a quiz by K. Russell and P. Carter.
 * https://oeis.org/A122472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 983;   a(n+1) = (a(n)/10) * (a(n)%10) / 2  */
seq_t a(idx_t n)
{
    seq_t val = 983;               /* a(0) */
    if (n == 0) return val;

    /* Apply the recurrence until the requested index or until result
       becomes zero – afterwards all terms are zero. */
    for (idx_t i = 1; i <= n && val > 0; ++i)
    {
        seq_t high = val / 10;     /* all digits except the last */
        seq_t low  = val % 10;     /* last digit */
        val = (high * low) / 2;
    }
    return val;
}
