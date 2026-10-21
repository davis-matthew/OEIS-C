/*
 * A057300  Binary counter with odd/even bit positions swapped; base-4 counter
 *          with 1's replaced by 2's and vice versa.
 * https://oeis.org/A057300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(4*n + k) = 4*a(n) + a(k) (0 ≤ k ≤ 3) Base values: a(0) = 0,
 *            a(1) = 2, a(2) = 1, a(3) = 3 This implements the OEIS sequence
 *            A057300
 * Range:     u8 n <= 255, u16 n <= 9857, u32 n <= 9857, u64 n <= 9857,
 *            u128 n <= 9857, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases for 0 ≤ n < 4 */
    if (n < 4) {
        static const seq_t base[4] = { 0, 2, 1, 3 };
        return base[n];
    }

    /* Apply the recurrence recursively:
       n = 4 * (n / 4) + (n % 4)   →   a(n) = 4 * a(n/4) + a(n%4)   */
    return (seq_t)4 * a(n / 4) + a(n % 4);
}
