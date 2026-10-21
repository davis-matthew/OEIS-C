/*
 * A056980  Number of blocks of {1, 1, 0} in binary expansion of n.
 * https://oeis.org/A056980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of occurrences of the binary pattern "110" in the
 *            binary expansion of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(2n)   = a(n) + [n ≡ 3 (mod 4)],   a(2n+1) = a(n)   (for n ≥ 1) */

seq_t a(idx_t n)
{
    /* Count how many triplets 110 appear in the binary representation of n. */
    if (n < 6)            /* less than 3 bits, cannot contain "110" */
        return (seq_t)0;

    /* Determine the number of bits in n */
    idx_t tmp = n;
    int bits = 0;
    while (tmp) {
        tmp >>= 1;
        ++bits;
    }

    int count = 0;
    /* Scan each possible starting position of a 3‑bit window */
    for (int i = bits - 3; i >= 0; --i) {
        if ( ((n >> i) & 7) == 6 )   /* 7 = 0b111, 6 = 0b110 */
            ++count;
    }
    return (seq_t)count;
}
