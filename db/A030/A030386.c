/*
 * A030386  Triangle T(n,k): write n in base 4, reverse order of digits.
 * https://oeis.org/A030386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf, less
 * Formula:   a(n) is the n-th digit in the concatenation of reversed base‑4
 *            representations of all nonnegative integers
 * Range:     u8 n <= 4664, u16 n <= 4664, u32 n <= 4664, u64 n <= 4664,
 *            u128 n <= 4664, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t pos = n;     /* position in the global digit stream */
    idx_t num = 0;     /* current integer whose reversed digits we are scanning */

    while (1) {
        /* Determine the number of digits of `num` in base 4 */
        idx_t tmp = num;
        int digits = 1;            /* at least one digit for zero */
        while (tmp >= 4) {
            tmp /= 4;
            digits++;
        }

        /* Does the desired digit lie within the digits of `num`? */
        if (pos < (idx_t)digits) {
            /* Compute the digit at position `pos` in the reversed representation.
               Reversed representation has the least‑significant base‑4 digit first,
               so the pos‑th digit is the (pos)-th digit from the least significant side. */
            idx_t pow = 1;
            for (int i = 0; i < pos; ++i)   /* pow = 4^pos */
                pow *= 4;

            seq_t digit = (seq_t)((num / pow) % 4);
            return digit;
        }

        /* Skip all digits of `num` and move to the next integer */
        pos -= digits;
        num++;
    }
}
