/*
 * A071521  Number of 3-smooth numbers <= n.
 * https://oeis.org/A071521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, changed
 * Formula:   a(n) = Sum_{i=0}^{⌊log₂ n⌋} (⌊log₃(n / 2^i)⌋ + 1) Number of
 *            3‑smooth numbers ≤ n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t p2 = 1; p2 <= n; ) {
        /* Count powers of 3 not exceeding n / p2. */
        idx_t t = n / p2;
        for (idx_t tmp = t; tmp; tmp /= 3) {
            ++count;          /* each tmp represents one 3^k.   */
        }
        /* Prepare next power of 2, avoiding overflow. */
        if (p2 > n / 2) break;
        p2 *= 2;
    }
    return count;
}
