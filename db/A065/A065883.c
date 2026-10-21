/*
 * A065883  Remove factors of 4 from n (i.e., write n in base 4, drop final
 *          zeros, then rewrite in decimal).
 * https://oeis.org/A065883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, mult
 * Formula:   a(n) = n / 4^k , where 4^k is the largest power of 4 dividing n
 * Range:     u8 n <= 256, u16 n <= 9285, u32 n <= 9285, u64 n <= 9285,
 *            u128 n <= 9285, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to: if n % 4 == 0 then a(n) = a(n/4) else a(n) = n */

seq_t a(idx_t n) {
    /* Work on an unsigned integer that can represent n. */
    idx_t tmp = n;
    /* Divide by 4 while n is a multiple of 4.  Dividing by 4 is a right shift of 2 bits. */
    while ((tmp & 3ULL) == 0ULL) {
        tmp >>= 2;
    }
    /* Convert back to seq_t for the return value. */
    return (seq_t)tmp;
}
