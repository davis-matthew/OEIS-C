/*
 * A084967  Multiples of 5 whose GCD with 6 is 1.
 * https://oeis.org/A084967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*n - (n even ? 5 : 10) The terms are the multiples of 5
 *            that are coprime to 6, which are exactly 5 times the numbers not
 *            divisible by 2 or 3. Using 15*n - (n even ? 5 : 10) yields the
 *            same result as a(n) = 5*(-3 + (-1)^n + 6*n)/2. This
 *            implementation works for any n ≥ 1 and relies only o...
 * Range:     u8 n <= 17, u16 n <= 4369, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is at least 1.  term = 15*n will fit the required result
       as long as the final value fits in seq_t.  The subtraction
       keeps the value non‑negative. */
    seq_t term = (seq_t)15 * n;
    if ((n & 1) == 0)
        term -= (seq_t)5;    /* even n: a(n) = 15*n - 5  */
    else
        term -= (seq_t)10;   /* odd n : a(n) = 15*n -10 */
    return term;
}
