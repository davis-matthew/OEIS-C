/*
 * A045801  1-ish numbers (end in 11, 39, 73, 77).
 * https://oeis.org/A045801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 100 * floor((n-1)/4) + {11,39,73,77}[ (n-1) % 4 ]
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the n-th number whose decimal representation ends in
   11, 39, 73 or 77.  The sequence is 11,39,73,77,111,139,… in
   increasing order.  */
seq_t a(idx_t n)
{
    /* index of the second digit group 0..3 */
    static const seq_t endings[4] = {11, 39, 73, 77};

    idx_t blocks = (n - 1) / 4;   /* how many full 100‑blocks precede */
    idx_t rem    = (n - 1) % 4;   /* which ending within that block */

    seq_t base = (seq_t)blocks * 100;   /* 100 * blocks  */
    return base + endings[rem];
}
