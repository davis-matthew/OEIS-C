/*
 * A072675  Integers m such that the last digit of Fibonacci(m) is 1.
 * https://oeis.org/A072675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 60*floor((n-1)/8) + [1,2,8,19,22,28,41,59][(n-1)%8]
 * Range:     u8 n <= 35, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=1 is the first term; computed without any lookup tables. */

seq_t a(idx_t n)
{
    /* Residues of positions modulo 60 where Fibonacci has last digit 1 */
    static const seq_t residues[8] = {
        (seq_t)1, (seq_t)2, (seq_t)8, (seq_t)19,
        (seq_t)22, (seq_t)28, (seq_t)41, (seq_t)59
    };

    idx_t k = (n - 1) / 8;          /* number of full periods of 60 */
    idx_t r = (n - 1) % 8;          /* index into residues */

    /* Compute term in seq_t type */
    seq_t term = ((seq_t)60 * k) + residues[r];
    return term;
}
