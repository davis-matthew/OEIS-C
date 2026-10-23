/*
 * A107681  Repeat(first 2^k numbers with no 2 in ternary representation) for
 *          k>0.
 * https://oeis.org/A107681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = f(n - (2^k - 1)) where k = ⌊log₂(n+1)⌋, and f(x) maps a
 *            binary number x to a base‑3 number with the same bits (sum of
 *            x·3^i over set bits i). This implements the concatenation of
 *            blocks of length 2^k of the sequence “numbers that in base 3
 *            contain no digit 2”
 * Range:     u8 n <= 101, u16 n <= 3326, u32 n <= 8190, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find k such that 2^k ≤ n+1 < 2^(k+1).  */
    idx_t tmp = n + 1;
    idx_t k = 0;
    while (tmp > 1) {
        tmp >>= 1;
        ++k;
    }

    /* Start position of the k‑th block: 2^k – 1.  */
    seq_t start = ((seq_t)1 << k) - 1;

    /* Offset inside the block.  */
    seq_t d = (seq_t)n - start;

    /* Convert binary d to base‑3 with the same bit pattern.  */
    seq_t result = 0;
    seq_t pow3   = 1;          /* 3^0 */
    while (d) {
        if (d & 1)
            result += pow3;
        pow3 *= 3;
        d >>= 1;
    }
    return result;
}
