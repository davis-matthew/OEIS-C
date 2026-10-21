/*
 * A070502  a(n) = n^3 mod 40.
 * https://oeis.org/A070502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 40
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n may be as large as 2^64-1, so use 64‑bit arithmetic for intermediate
       computations before converting back to seq_t (which may be 8, 16, 32,
       64 or 128 bits).  The result is always < 40, so the cast is safe. */
    uint64_t m = (uint64_t)n % 40ULL;
    uint64_t t = (m * m) % 40ULL;
    t      = (t * m) % 40ULL;
    return (seq_t)t;
}
