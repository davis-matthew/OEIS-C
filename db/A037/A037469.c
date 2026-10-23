/*
 * A037469  Duplicate of A007091.
 * https://oeis.org/A037469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = decimal interpretation of the base‑5 representation of n
 * Range:     i8 n <= 39, i16 n <= 53, i32 n <= 53, i64 n <= 53, i128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;      /* accumulated decimal value */
    seq_t power  = 1;      /* 10^position */
    idx_t  m = n;          /* local copy of index */

    while (m > 0) {
        unsigned digit = m % 5;        /* least significant base‑5 digit */
        result += (seq_t)digit * power;
        power  *= 10;                  /* move to next decimal place */
        m      /= 5;                   /* shift right in base‑5 */
    }

    return result;
}
