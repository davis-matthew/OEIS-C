/*
 * A047367  Numbers that are congruent to {0, 1, 3, 4, 5} mod 7.
 * https://oeis.org/A047367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)/5)*7 + [0,1,3,4,5][(n-1)%5]
 * Range:     u8 n <= 183, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based.  Compute cycles of 5 allowed numbers in each
       block of 7 natural numbers. */
    idx_t i = n - 1;
    idx_t cycles = i / 5;
    idx_t rem = i % 5;

    /* Offsets for the 5 allowed residues: 0,1,3,4,5 mod 7. */
    static const seq_t offs[5] = {
        (seq_t)0,
        (seq_t)1,
        (seq_t)3,
        (seq_t)4,
        (seq_t)5
    };

    return (seq_t)cycles * (seq_t)7 + offs[rem];
}
