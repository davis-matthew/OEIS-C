/*
 * A010071  a(n+1) = a(n) + sum of digits in base 8 representation of a(n).
 * https://oeis.org/A010071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(n+1) = a(n) + sum_of_digits_base8(a(n)) */
seq_t a(idx_t n)
{
    seq_t value = (seq_t)1;                  /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {         /* compute a(1) … a(n)   */
        seq_t tmp = value;
        seq_t sum = (seq_t)0;
        while (tmp != (seq_t)0) {           /* sum base‑8 digits     */
            seq_t digit = tmp % (seq_t)8;
            sum += digit;
            tmp /= (seq_t)8;
        }
        value += sum;                       /* a(i) = a(i-1)+sum    */
    }
    return value;                            /* a(n)                  */
}
