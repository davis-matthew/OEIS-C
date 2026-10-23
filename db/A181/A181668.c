/*
 * A181668  Period 12: repeat [5,5,5,2,2,2,8,8,8,2,2,2].
 * https://oeis.org/A181668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = a(n-1) - a(n-3) + a(n-4) - a(n-6) + a(n-7) - a(n-9) +
 *            a(n-10), which yields the periodic sequence with period 12:
 *            [5,5,5,2,2,2,8,8,8,2,2,2]
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1; safe fallback */

    static const seq_t pat[12] = {
        5, 5, 5, 2, 2, 2, 8, 8, 8, 2, 2, 2
    };

    unsigned idx = (unsigned)((n - 1ULL) % 12ULL);
    return pat[idx];
}
