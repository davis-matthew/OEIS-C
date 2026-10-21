/*
 * A059302  A diagonal of A008296.
 * https://oeis.org/A059302
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n-1)*n*(n+1)*(3n-10)/24 (n ≥ 2)
 * Range:     i8 n <= 4, i16 n <= 11, i32 n <= 197, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t once to avoid reinterpret cast in every operation */
    seq_t nn = (seq_t)n;
    seq_t t1 = nn - 1;           /* n-1 */
    seq_t t2 = nn;               /* n   */
    seq_t t3 = nn + 1;           /* n+1 */
    seq_t t4 = 3 * nn - 10;      /* 3n-10 */

    /* Compute the product in a manner that stays within native precision for 64‑bit builds.
       For n up to at least 10,000 the product fits in 64‑bit; for larger n
       arbitrary‑precision seq_t will handle it automatically. */
    seq_t prod = t1 * t2;
    prod = prod * t3;
    prod = prod * t4;

    /* Final division by 24, exact for all integers n. */
    return prod / 24;
}
