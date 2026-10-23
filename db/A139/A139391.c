/*
 * A139391  Next odd term in Collatz trajectory with starting value n.
 * https://oeis.org/A139391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Next odd term in Collatz trajectory starting at n */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    /* Apply the Collatz rule once if n is odd */
    if ((x & (seq_t)1) != 0) {
        x = 3 * x + (seq_t)1;
    }
    /* Keep dividing by 2 until an odd number is reached */
    while ((x & (seq_t)1) == 0) {
        x >>= 1;
    }
    return x;
}
