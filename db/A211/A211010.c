/*
 * A211010  Value on the axis "x" of the endpoint of the structure of A211000
 *          at n-th stage.
 * https://oeis.org/A211010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) for n > 4; a(0)=0, a(1)=1,
 *            a(2)=2, a(3)=3, a(4)=4
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* For n >= 2: the sequence repeats 2, 3, 4, 3 with period 4. */
    switch (n & 3U) {
        case 0:  return (seq_t)4;  /* n % 4 == 0  → 4  (e.g., n = 4, 8, ...) */
        case 2:  return (seq_t)2;  /* n % 4 == 2  → 2  (e.g., n = 2, 6, ...) */
        default: return (seq_t)3;  /* odd indices n >= 3 → 3 */
    }
}
