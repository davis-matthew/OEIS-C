/*
 * A072709  Integers m such that the last digit of Fibonacci(m) is 7.
 * https://oeis.org/A072709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = residues[(n-1)%8] + 60 * floor((n-1)/8) where residues =
 *            [14,16,17,23,34,37,43,56]
 * Range:     u8 n <= 33, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A072709 */
seq_t a(idx_t n)
{
    /* Residues where Fibonacci(m) ends in 7, modulo 60 */
    static const seq_t residues[8] = {
        (seq_t)14, (seq_t)16, (seq_t)17, (seq_t)23,
        (seq_t)34, (seq_t)37, (seq_t)43, (seq_t)56
    };

    /* Compute the block index and intra-block residue index */
    idx_t block   = (n - 1) / 8;                 /* floor((n-1)/8)  */
    int   offset  = (int)((n - 1) % 8);          /* (n-1) mod 8, 0..7 */

    /* Compute the term: residue + 60*block */
    seq_t result = residues[offset];
    result += (seq_t)block * (seq_t)60;

    return result;
}
