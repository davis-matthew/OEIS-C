/*
 * A085270  Integer part of the conversion from miles to kilometers.
 * https://oeis.org/A085270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( 160934*n / 100000 ) (integer part of miles to km)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Perform the division in two parts to avoid overflow on 64‑bit builds.
       For arbitrary‑precision builds the casts are harmless. */
    idx_t q = n / 100000ULL;
    idx_t r = n % 100000ULL;
    seq_t part1 = (seq_t)q * 160934ULL;
    seq_t part2 = (seq_t)r * 160934ULL / 100000ULL;
    return part1 + part2;
}
