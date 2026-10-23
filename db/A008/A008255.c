/*
 * A008255  Coordination sequence T2 for feldspar.
 * https://oeis.org/A008255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n ≥ 1 let m = n / 3. a(3*m) = 20*m^2 + 2 a(3*m +
 *            1) = 20*m^2 + 14*m + 4 a(3*m + 2) = 20*m^2 + 26*m + 10
 * Range:     u8 n <= 10, u16 n <= 171, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t m = (seq_t)(n / 3);
    uint64_t r = n % 3;

    switch (r) {
    case 0:
        return (seq_t)20 * m * m + 2;
    case 1:
        return (seq_t)20 * m * m + (seq_t)14 * m + 4;
    case 2:
        return (seq_t)20 * m * m + (seq_t)26 * m + 10;
    }

    /* Unreachable, but keep compiler happy */
    return 0;
}
