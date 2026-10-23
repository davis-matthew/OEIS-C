/*
 * A104770  Expansion of g.f. (1+x^2)/(1+x-x^3).
 * https://oeis.org/A104770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 32, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+3) = a(n) - a(n+2);  a(0)=1, a(1)=-1, a(2)=2. */
/* This implementation uses a small ring buffer of size 4; no dynamic */
/* allocation is required and it works for any seq_t type that supports */
/* the usual arithmetic operators. */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)2;

    /* Ring buffer of the last four computed values. */
    seq_t buf[4];
    buf[0] = (seq_t)1;   /* a(0) */
    buf[1] = (seq_t)-1;  /* a(1) */
    buf[2] = (seq_t)2;   /* a(2) */
    /* buf[3] will be written by the loop below. */

    for (idx_t i = 3; i <= n; ++i)
    {
        /* i-3 and i-1 are always non‑negative and less than i.  */
        buf[i % 4] = buf[(i - 3) % 4] - buf[(i - 1) % 4];
    }

    return buf[n % 4];
}
