/*
 * A067898  Least digit not used in n (or 10 if n is pandigital).
 * https://oeis.org/A067898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest nonnegative decimal digit not present in * the
 *            decimal expansion of n, or 10 if all digits 0–9 appear
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    unsigned mask = 0;                /* bit d set if digit d occurs */
    if (n == 0) {
        mask |= 1u;                   /* digit 0 is present for n = 0 */
    } else {
        while (n > 0) {
            mask |= 1u << (n % 10);
            n /= 10;
        }
    }
    for (int d = 0; d < 10; ++d) {
        if (!(mask & (1u << d)))
            return (seq_t)d;
    }
    return (seq_t)10;                  /* n is pandigital */
}
