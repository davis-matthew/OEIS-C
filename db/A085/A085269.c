/*
 * A085269  Integer part of the conversion from centimeters to inches.
 * https://oeis.org/A085269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n/2.54) = floor(n*50/127)
 * Range:     u8 n <= 650, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the integer part of centimeters to inches conversion.  
   Uses integer arithmetic only to avoid floating point or large
   intermediate values.  The computation is safe for all values
   that fit into the 64‑bit idx_t type. */
seq_t a(idx_t n)
{
    /* Integer division:  floor(n*50/127) = (n/127)*50 + (n%127)*50/127   */
    idx_t q = n / 127;
    idx_t r = n % 127;
    /* q*50 and (r*50)/127 are guaranteed to fit within 64 bits      */
    return (seq_t)(q * 50 + (r * 50) / 127);
}
