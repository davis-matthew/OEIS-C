/*
 * A060904  Largest power of 5 that divides n.
 * https://oeis.org/A060904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = largest power of 5 dividing n (i.e., a(n) = 5^k where k =
 *            max{ k | 5^k divides n })
 * Range:     u8 n <= 624, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;               /* 5^0 = 1 for n not divisible by 5 */
    idx_t m = n;                 /* use a copy of the index */
    while (m % 5 == 0)          /* test for divisibility by 5 */
    {
        res *= 5;                /* multiply the power of five we keep track of */
        m /= 5;                  /* remove one factor of five from n */
    }
    return res;
}
