/*
 * A130734  List of numbers of cents you can have in US coins without having
 *          change for a dollar.
 * https://oeis.org/A130734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   a(n) = n-1 for 1 ≤ n ≤ 100 * a(n) = 104 + (n-100) for 101 ≤ n ≤
 *            105 * a(n) = 114 + (n-105) for 106 ≤ n ≤ 110 * a(n) = 0
 *            (undefined) for n > 110
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 100ULL)
        return (seq_t)(n - 1ULL);
    else if (n <= 105ULL)
        return (seq_t)(104ULL + (n - 100ULL));
    else if (n <= 110ULL)
        return (seq_t)(114ULL + (n - 105ULL));
    else
        return (seq_t)0;
}
