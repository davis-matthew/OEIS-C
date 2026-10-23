/*
 * A048441  Take the first n numbers written in base 9, concatenate them, then
 *          convert from base 9 to base 10.
 * https://oeis.org/A048441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the base‑9 integer obtained by concatenating the *
 *            base‑9 representations of 1, 2, …, n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset 1, but handle 0 safely */

    seq_t result = 0;                         /* current concatenated value */
    seq_t pow9   = 9;                         /* 9^digits of the current number */
    idx_t next_threshold = 9;                 /* first number that increases digit count */

    for (idx_t i = 1; i <= n; ++i) {
        /* If i is the first number with a new digit length, update pow9. */
        if (i == next_threshold) {
            next_threshold *= 9;
            pow9 *= 9;
        }
        /* Append the current number i (in base 9, its value is i). */
        result = result * pow9 + (seq_t)i;
    }

    return result;
}
