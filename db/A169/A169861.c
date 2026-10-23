/*
 * A169861  Ceiling of n inches converted to millimeters.
 * https://oeis.org/A169861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n*25.4) = ceil(n*127 / 5)
 * Range:     u8 n <= 10, u16 n <= 2580, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute ceil(n*127/5) safely using 128‑bit arithmetic. */
    unsigned __int128 prod = (unsigned __int128)n * (unsigned __int128)127;
    prod += 4;               /* add (5-1) for ceiling */
    return (seq_t)(prod / 5);
}
