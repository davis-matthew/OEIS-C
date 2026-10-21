/*
 * A217096  Characteristic function of numbers that have a nonleading zero in
 *          their decimal representation (A011540). 0 itself is also included,
 *          so a(0) = 1.
 * https://oeis.org/A217096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if n = 0 or the decimal representation of n contains a
 *            zero digit (non‑leading); otherwise 0
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    idx_t x = n;
    while (x > 0) {
        if ((x % 10) == 0) {
            return (seq_t)1;
        }
        x /= 10;
    }
    return (seq_t)0;
}
