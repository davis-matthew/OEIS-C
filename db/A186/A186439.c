/*
 * A186439  Numbers whose squares end in three identical digits.
 * https://oeis.org/A186439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 917, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A186439: Numbers whose squares end in three identical digits.
 * A(n) can be expressed by a 7‑term recurrence:
 *     a(1) = 38,   a(2) = 100,  a(3) = 200,  a(4) = 300,
 *     a(5) = 400,  a(6) = 462,  a(7) = 500,
 *     a(n) = a(n-7) + 500   for n > 7.
 *
 * The closed form derived from this pattern is:
 *     let r = (n-1) mod 7,   k = floor((n-1)/7)
 *     a(n) = base[r] + 500 * k
 * with base[0..6] = {38,100,200,300,400,462,500}.
 */
seq_t a(idx_t n)
{
    /* compute (n-1)/7 and (n-1)%7 */
    idx_t n1 = n - 1;           /* safe: n >= 1 */
    idx_t k  = n1 / 7;          /* number of full 500‑increments */
    idx_t r  = n1 % 7;          /* position in the 7‑term base cycle */

    /* static base values fit in any seq_t */
    static const seq_t base[7] = {38, 100, 200, 300, 400, 462, 500};

    /* assemble the result using seq_t arithmetic only */
    seq_t result = base[r] + (seq_t)500 * (seq_t)k;
    return result;
}
