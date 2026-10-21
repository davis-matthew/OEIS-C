/*
 * A091084  a(n) = A001045(n) mod 10.
 * https://oeis.org/A091084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-4) for n > 4 with initial values * a(0) = 0, a(1) =
 *            1, a(2) = 1, a(3) = 3, a(4) = 5. * Hence for n ≥ 1, a(n) repeats
 *            the cycle {1,1,3,5}
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special handling for the offset 0. */
    if (n == 0u)
        return (seq_t)0;

    /* The repeating cycle for n ≥ 1. */
    static const seq_t cycle[4] = { (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)5 };

    /* n-1 is always safe here because n != 0, and we keep the index
     * in the range [0,3] using a bitwise AND. */
    return cycle[(n - 1u) & 3u];
}
