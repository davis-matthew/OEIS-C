/*
 * A126006  Involution of nonnegative integers: Swap the positions of digits
 *          q0 <-> q1, q2 <-> q3, q4 <-> q5, etc. in the base-4 expansion of n
 *          (where n = ... + q4*256 + q3*64 + q2*16 + q1*4 + q0).
 * https://oeis.org/A126006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by swapping every adjacent pair of base‑4
 *            digits of n. That is, for the base‑4 expansion n = ... + q4·4^4
 *            + q3·4^3 + q2·4^2 + q1·4 + q0, a(n) = ... + q3·4^4 + q4·4^3 +
 *            q1·4^2 + q2·4 + q0
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t cur   = n;           /* current part of n being processed */
    seq_t power = 1;           /* current 4^pos, starting with 4^0 = 1 */

    while (cur > 0) {
        /* Extract the first digit of the current pair (least significant) */
        idx_t d0 = cur & 3;   /* cur % 4 */
        cur >>= 2;            /* cur /= 4 */

        /* Extract the second digit of the pair, or 0 if it doesn't exist */
        idx_t d1 = 0;
        if (cur > 0) {
            d1 = cur & 3;     /* second digit */
            cur >>= 2;        /* move to the next pair */
        }

        /* After swapping, place d1 in the low position, d0 in the high */
        result += (seq_t)d1 * power;
        power <<= 2;                     /* multiply by 4 */
        result += (seq_t)d0 * power;
        power <<= 2;                     /* next pair will start 4 positions higher */
    }
    return result;
}
