/*
 * A192833  Molecular topological indices of the Moebius ladders.
 * https://oeis.org/A192833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3/2)*n*(9 -(-1)^n +4*n +2*n^2)
 * Range:     u8 n <= 3, u16 n <= 27, u32 n <= 901, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    seq_t base = ((n & (idx_t)1) ? (seq_t)10 : (seq_t)8);
    seq_t n2 = t * t;
    seq_t part = base + ((seq_t)4 * t) + ((seq_t)2 * n2);
    seq_t result = (((seq_t)3 * t) * part) / (seq_t)2;
    return result;
}
