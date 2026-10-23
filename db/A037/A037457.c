/*
 * A037457  Duplicate of A037314.
 * https://oeis.org/A037457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = interpret the base‑3 expansion of n as a base‑9 number
 * Range:     i8 n <= 17, i16 n <= 20, i32 n <= 20, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    /* store base‑3 digits (least significant first) */
    int digits[64];
    int len = 0;
    idx_t temp = n;
    while (temp > 0) {
        digits[len++] = (int)(temp % 3);
        temp /= 3;
    }
    /* build the result in base‑9, most significant digit first */
    for (int i = len - 1; i >= 0; --i) {
        res = res * 9 + (seq_t)digits[i];
    }
    return res;
}
