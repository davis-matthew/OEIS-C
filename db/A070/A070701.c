/*
 * A070701  a(n) = n^7 mod 19.
 * https://oeis.org/A070701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 19
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 19 first.  Since 19 is a small prime, the intermediate
       values are all < 19^7 which comfortably fits in a 64‑bit unsigned type. */
    uint64_t r = n % 19ULL;

    uint64_t res = 1ULL;
    for (int i = 0; i < 7; ++i)
        res = (res * r) % 19ULL;

    return (seq_t)res;
}
