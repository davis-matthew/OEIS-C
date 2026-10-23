/*
 * A040815  Continued fraction for sqrt(845).
 * https://oeis.org/A040815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 29; for n ≥ 1, a(n) repeats with period 5: 14,1,1,14,58
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)29;
    idx_t r = (n - 1) % 5;
    switch (r) {
    case 0: return (seq_t)14;
    case 1: return (seq_t)1;
    case 2: return (seq_t)1;
    case 3: return (seq_t)14;
    case 4: return (seq_t)58;
    default: return (seq_t)0; /* unreachable */
    }
}
