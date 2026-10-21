/*
 * A070583  a(n) = n^4 mod 40.
 * https://oeis.org/A070583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 40
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute n mod 40 first – the remaining calculation fits easily in 64‑bit. */
    uint64_t r = n % 40ULL;
    /* Square and reduce modulo 40 twice to obtain r^4 mod 40. */
    uint64_t r2 = (r * r) % 40ULL;
    uint64_t r4 = (r2 * r2) % 40ULL;
    return (seq_t)r4;
}
