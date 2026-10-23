/*
 * A029926  Convert n from degrees Fahrenheit to nearest integer Celsius.
 * https://oeis.org/A029926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = round(5*(n-32)/9)
 * Range:     i8 none, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fn     = (seq_t)n;
    seq_t delta  = fn - (seq_t)32; /* n - 32 */
    seq_t prod   = 5 * delta;      /* 5*(n-32) */
    seq_t result;

    if (prod >= 0)
        result = (prod + 4) / 9;   /* round for non‑negative */
    else
        result = (prod - 4) / 9;   /* round for negative */

    return result;
}
