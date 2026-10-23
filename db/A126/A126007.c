/*
 * A126007  Involution of nonnegative integers: Keep the least significant
 *          quaternary digit q0 of n fixed, but swap the positions of digits
 *          q1 <-> q2, q3 <-> q4, ..., etc. in the base-4 expansion of n
 *          (where n = ... + q4*256 + q3*64 + q2*16 + q1*4 + q0).
 * https://oeis.org/A126007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n mod 4) + 4 * A126006(floor(n/4)) Implements the
 *            involution of non‑negative integers by swapping base‑4 digits in
 *            pairs (q1<->q2, q3<->q4, …) while keeping q0 fixed
 * Range:     u8 n <= 63, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t power = 1;                   /* 4^position */

    /* least significant quaternary digit q0 stays the same */
    seq_t q0 = (seq_t)(n & 3ULL);
    res += q0 * power;
    power *= 4;                        /* now power = 4^1 */

    /* process remaining digits in pairs (q1, q2), (q3, q4), … */
    idx_t m = n >> 2;                  /* drop the q0 digit */

    while (m != 0) {
        seq_t d1 = (seq_t)(m & 3ULL);  /* current lower digit of the pair */
        m >>= 2;                       /* move to next higher digits */
        seq_t d2 = 0;
        if (m != 0) {
            d2 = (seq_t)(m & 3ULL);    /* next digit of the pair */
            m >>= 2;
        }

        /* after swapping, d2 occupies the lower position of the pair */
        res += d2 * power;
        power *= 4;                    /* power now = 4^next_position */
        res += d1 * power;
        power *= 4;                    /* power ready for following pair */
    }

    return res;
}
