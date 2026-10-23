/*
 * A167387  a(n) = (-1)^(n+1) * n*(n-1)*(n-4)*(n+1)/12.
 * https://oeis.org/A167387
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^(n+1) * n*(n-1)*(n-4)*(n+1)/12 (offset 2)
 * Range:     i8 n <= 5, i16 n <= 14, i32 n <= 315, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the OEIS offset does not define a value.
       Return 0 for completeness. */
    if (n < 2)
        return 0;

    seq_t ns = (seq_t)n;                     /* convert index to seq_t */
    seq_t prod = ns * (ns - 1) * (ns - 4) * (ns + 1);  /* n*(n-1)*(n-4)*(n+1) */
    seq_t val  = prod / 12;                  /* division is exact for n ≥ 2 */

    /* Apply (-1)^(n+1).  Since n+1 odd ⇔ n even, use n%2. */
    if ((n & 1) == 0)                       /* n even → sign = -1 */
        val = -val;

    return val;
}
