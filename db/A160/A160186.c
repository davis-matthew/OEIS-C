/*
 * A160186  Lodumo_5 of Lucas numbers.
 * https://oeis.org/A160186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 + 5 * (n / 4) + delta(n mod 4) where delta(0) = 0,
 *            delta(1) = –1, delta(2) = +1, delta(3) = +2 This sequence
 *            satisfies a(0)=2, a(1)=1, a(2)=3, a(3)=4, a(4)=7, … It is
 *            equivalent to a(n) = lod_5(A000032(n)) for all n ≥ 0
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t idx = (seq_t)n;

    /* One full cycle of 4 terms changes the value by +5 */
    seq_t period = idx / 4;
    seq_t rem    = idx % 4;

    /* Base value after complete cycles */
    seq_t val = (seq_t)2 + period * (seq_t)5;

    /* Apply the remainder-specific adjustment */
    if (rem == 1)
        val -= (seq_t)1;          /* delta = –1 */
    else if (rem == 2)
        val += (seq_t)1;          /* delta = +1 */
    else if (rem == 3)
        val += (seq_t)2;          /* delta = +2 */
    /* rem == 0 → delta = 0, nothing to add */

    return val;
}
