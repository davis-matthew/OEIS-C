/*
 * A070625  a(n) = n^5 mod 42.
 * https://oeis.org/A070625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 42
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 42 first to keep numbers small */
    uint64_t nm = n % 42ULL;          /* nm ∈ [0,41] */
    uint64_t r = nm;                  /* r = nm^1 mod 42 */
    /* Multiply four more times, taking the modulus each step to avoid overflow */
    r = (r * nm) % 42ULL;             /* r = nm^2 mod 42 */
    r = (r * nm) % 42ULL;             /* r = nm^3 mod 42 */
    r = (r * nm) % 42ULL;             /* r = nm^4 mod 42 */
    r = (r * nm) % 42ULL;             /* r = nm^5 mod 42 */
    return (seq_t)r;
}
