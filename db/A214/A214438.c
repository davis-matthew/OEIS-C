/*
 * A214438  Numerator of correlation kernels arising in adding a list of
 *          numbers in base 3 considering the distribution of number of
 *          carries.
 * https://oeis.org/A214438
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign, frac, easy
 * Formula:   a(n) = 0 for n<-1; a(-1)=1, a(0)=1, a(1)=2. For n ≥ 2: a(n)=0 if
 *            n ≡ 4 (mod 6); otherwise a(n)=(-1)^{⌊(n+1)/6⌋} * 2^{δ(n)}, where
 *            δ(n)=1 if n ≡ 1 (mod 6) and δ(n)=0 otherwise
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < -1) return (seq_t)0;          /* k(n)=0 for n<-1 */

    if (n == -1) return (seq_t)1;          /* k(-1)=1/3 → numerator 1 */
    if (n == 0)  return (seq_t)1;          /* base value */
    if (n == 1)  return (seq_t)2;          /* base value */

    /* n ≥ 2 */
    if ((n - 4) % 6 == 0) return (seq_t)0; /* n = 4 + 6j → zero */

    /* parity of floor((n+1)/6) */
    int parity = (((n + 1) / 6) % 2) == 0 ? 1 : -1; /* +1 or -1 */

    /* factor 2^{δ(n)} : δ(n)=1 if n≡1 (mod 6) */
    int factor = (n % 6 == 1) ? 2 : 1;

    return (seq_t)(parity * factor);
}
