/*
 * A037638  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 2,1,3.
 * https://oeis.org/A037638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*a(n-1) + d(n) where d(n) = 2 if n≡1 (mod 3), 1 if n≡2
 *            (mod 3), 3 if n≡0 (mod 3). Initial value: a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – behaviour below offset is arbitrary */
    if (n == 1) return (seq_t)2;   /* a(1) = 2 */

    seq_t res = (seq_t)2;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t digit = (seq_t)(
            (i % 3 == 1) ? 2 :
            (i % 3 == 2) ? 1 : 3); /* cycle 2,1,3 */
        res = res * (seq_t)10 + digit;
    }
    return res;
}
