/*
 * A070664  a(n) = n^6 mod 41.
 * https://oeis.org/A070664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 41
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Reduce n modulo 41 first – the result will be < 41 */
    uint64_t r = n % 41ULL;

    /* Compute r^6 modulo 41 using only small intermediates */
    uint64_t r2 = (r * r) % 41ULL;     /* r^2 */
    uint64_t r3 = (r2 * r) % 41ULL;    /* r^3 */
    uint64_t r6 = (r3 * r3) % 41ULL;   /* r^6 */

    return (seq_t) r6;
}
