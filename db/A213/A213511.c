/*
 * A213511  The sequence N'(n) arising in the enumeration of balanced ternary
 *          trees.
 * https://oeis.org/A213511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Let t = n-1. Let d be the smallest positive integer such that t
 *            < 3^1 + 3^2 + ⋯ + 3^d = (3^(d+1)-3)/2 . Then a(n) =
 *            Σ_{i=1}^{d-1} i·3^i + d·(t - Σ_{i=1}^{d-1} 3^i). This implements
 *            that formula using only integer arithmetic
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)               /* a(1) = 0 */
        return 0;

    /* t = number of increments that have been made up to index n */
    idx_t t = n - 1;

    /* pow  = 3^d                     (current power of 3)           */
    /* sumPow = Σ_{i=1}^{d} 3^i       (running sum of powers)        */
    /* sumI3i = Σ_{i=1}^{d} i·3^i    (running weighted sum)         */
    seq_t pow   = 3;
    seq_t sumPow = 3;
    seq_t sumI3i = 3;
    idx_t d = 1;              /* current block (diff value) */

    /* find the block d that contains t */
    while ((seq_t)t > sumPow) {
        ++d;
        pow   *= 3;            /* next power 3^d */
        sumPow += pow;         /* add new block length */
        sumI3i += (seq_t)d * pow; /* add its contribution */
    }

    /* sumPow now equals Σ_{i=1}^{d} 3^i.
       sumPrevPow = Σ_{i=1}^{d-1} 3^i  (sum of previous block lengths)   */
    seq_t sumPrevPow = sumPow - pow;
    /* sumPrevI3i = Σ_{i=1}^{d-1} i·3^i  (sum of previous block contributions) */
    seq_t sumPrevI3i = sumI3i - (seq_t)d * pow;

    /* t - sumPrevPow is the number of increments already applied in block d,
       including the current term. */
    seq_t result = sumPrevI3i + (seq_t)d * ((seq_t)t - sumPrevPow);
    return result;
}
