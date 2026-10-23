/*
 * A130508  a(1)=2. a(2)=3. a(3)=1. a(n+3) = 3 + a(n), for all positive
 *          integers n.
 * https://oeis.org/A130508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n+1 if n mod 3 != 0 * a(n) = n-2 if n mod 3 == 0 (derived
 *            from a(n+3)=3+a(n), a(1)=2,a(2)=3,a(3)=1)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 */
    if (n % 3 == 0)
        return (seq_t)(n - 2);
    else
        return (seq_t)(n + 1);
}
