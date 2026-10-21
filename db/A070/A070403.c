/*
 * A070403  a(n) = 7^n mod 9.
 * https://oeis.org/A070403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n mod 9, which cycles with period 3: n mod 3 = 0 → 1 n
 *            mod 3 = 1 → 7 n mod 3 = 2 → 4
 * Range:     u8 n <= 1001, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    switch (r) {
    case 0: return (seq_t)1;
    case 1: return (seq_t)7;
    case 2: return (seq_t)4;
    }
    return (seq_t)0; /* unreachable */
}
