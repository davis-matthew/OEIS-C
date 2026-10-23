/*
 * A140137  Numbers n such that A140080(n) = 1.
 * https://oeis.org/A140137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A140137: numbers n such that A140080(n) = 1.
   Here we recognize A140080(n) = 1 iff n is NOT divisible by 3
   and n cannot be written as the sum of two (distinct) powers of 2.
   Therefore a(n) is the n-th positive integer that is not divisible
   by 3 and whose binary representation does NOT contain exactly two
   1‑bits. */
static unsigned popcount_seq(seq_t x) {
    unsigned c = 0;
    while (x != 0) {
        x &= x - 1;   /* remove lowest set bit */
        ++c;
    }
    return c;
}

seq_t a(idx_t n) {
    idx_t seen = 0;
    seq_t x = 1;
    for (;;) {
        /* skip multiples of 3 */
        if (x % 3ULL != 0) {
            /* skip numbers that are sums of two distinct powers of 2
               i.e., numbers with exactly two 1‑bits */
            if (popcount_seq(x) != 2) {
                ++seen;
                if (seen == n) return x;
            }
        }
        ++x;
    }
}
