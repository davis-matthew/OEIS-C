/*
 * A064616  a(n) = (10^n-1)*(91/81)-n*10^n/9.
 * https://oeis.org/A064616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 10, i64 n <= 18, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A064616
   Recurrence: a(1) = 9
               a(n) = a(n-1) + (10 - n) * 10^(n-1)   for n >= 2  */
/* The recurrence is equivalent to the given closed form and requires
   only an O(n) iteration with a single multiplication by 10 at each
   step.  The function works for any seq_t (including arbitrary‑size
   integers) and any idx_t. */

seq_t a(idx_t n) {
    /* n is 1‑based; values for n <= 0 are not defined for the OEIS
       offset, but we return 0 to keep the function well defined. */
    if (n <= 0) return 0;
    if (n == 1) return (seq_t)9;

    seq_t result = (seq_t)9;          /* a(1) */
    seq_t pow10  = (seq_t)10;         /* 10^(1), needed for n = 2 */

    for (idx_t i = 2; i <= n; ++i) {
        /* term = (10 - i) * 10^(i-1) */
        seq_t coeff = (seq_t)10 - (seq_t)i;   /* allows negative values */
        result += coeff * pow10;
        pow10 *= (seq_t)10;                 /* advance to 10^i for next loop */
    }
    return result;
}
