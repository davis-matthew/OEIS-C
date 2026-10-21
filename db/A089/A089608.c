/*
 * A089608  a(n) = ((-1)^(n+1)*A002425(n)) modulo 6.
 * https://oeis.org/A089608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 9284, u16 n <= 9284, u32 n <= 9284, u64 n <= 9284,
 *            u128 n <= 9284, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 5 if the exponent of 2 in n is odd, 1 otherwise
   (equivalently a(n) = 5-4*A035263(n) modulo 6). */
seq_t a(idx_t n)
{
    /* Count the exponent of 2 in n (trailing zeros). */
    uint64_t tmp = (uint64_t)n;
    unsigned v2 = 0;
    while ((tmp & 1ULL) == 0ULL) {
        tmp >>= 1;
        ++v2;
    }
    return (v2 & 1U) ? (seq_t)5 : (seq_t)1;
}
