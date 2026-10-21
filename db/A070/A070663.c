/*
 * A070663  a(n) = n^6 mod 40.
 * https://oeis.org/A070663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 40
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Work only with 64‑bit values, the final result is <40 */
    uint64_t nm = (uint64_t)n % 40ULL;      /* n mod 40 */
    uint64_t t1 = (nm * nm) % 40ULL;       /* n^2      */
    uint64_t t2 = (t1 * nm) % 40ULL;       /* n^3      */
    uint64_t t3 = (t2 * nm) % 40ULL;       /* n^4      */
    uint64_t t4 = (t3 * nm) % 40ULL;       /* n^5      */
    uint64_t t5 = (t4 * nm) % 40ULL;       /* n^6      */
    return (seq_t)t5;
}
