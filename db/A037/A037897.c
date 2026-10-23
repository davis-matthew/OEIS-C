/*
 * A037897  a(n) = (Greatest base 3 digit of n) - (least base 3 digit of n).
 * https://oeis.org/A037897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (maximum base‑3 digit of n) - (minimum base‑3 digit of n)
 * Range:     u8 n <= 9935, u16 n <= 9935, u32 n <= 9935, u64 n <= 9935,
 *            u128 n <= 9935, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    idx_t tmp = n;
    seq_t maxd = 0;
    seq_t mind = 2;          /* largest possible base‑3 digit */

    while (tmp > 0) {
        seq_t digit = (seq_t)(tmp % 3);
        if (digit > maxd) {
            maxd = digit;
        }
        if (digit < mind) {
            mind = digit;
        }
        tmp = tmp / 3;
    }
    return maxd - mind;
}
