/*
 * A047262  Numbers that are congruent to {0, 2, 4, 5} mod 6.
 * https://oeis.org/A047262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*floor((n-1)/4) + {0,2,4,5}((n-1)%4)
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle n < 1 independently – behaviour not required, but safe */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Every block of 6 integers contains 4 sequence elements.
       The block index k = floor((n-1)/4), and the position r = (n-1)%4. */
    idx_t k = (n - 1) / 4;
    idx_t r = (n - 1) % 4;

    /* Compute the base of the block as 6 * k.
       Use idx_t for the multiplication to avoid overflow in very small
       seq_t types, then cast the final result to seq_t. */
    seq_t base = (seq_t)(6 * k);

    switch (r) {
        case 0:
            return base;               /* 6k          */
        case 1:
            return base + (seq_t)2;     /* 6k + 2      */
        case 2:
            return base + (seq_t)4;     /* 6k + 4      */
        case 3:
            return base + (seq_t)5;     /* 6k + 5      */
        default:
            return base; /* unreachable */
    }
}
