/*
 * A222243  In the number n, replace all (decimal) digits '4' with '8' and
 *          vice versa.
 * https://oeis.org/A222243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digit‑wise replace 4↔8 in the decimal representation of n
 * Range:     u8 n <= 239, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t place = 1;
    idx_t tmp = n;

    /* Handle n = 0 explicitly – the loop below would skip it */
    if (tmp == 0)
        return 0;

    while (tmp != 0) {
        unsigned d = (unsigned)(tmp % 10);
        unsigned nd = (d == 4) ? 8 : (d == 8 ? 4 : d);
        result += (seq_t)nd * place;
        place *= 10;
        tmp /= 10;
    }
    return result;
}
