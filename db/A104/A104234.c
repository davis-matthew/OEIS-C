/*
 * A104234  Number of k >= 1 such that k+n == 0 mod 2^k.
 * https://oeis.org/A104234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k ≥ 1 | (k + n) mod 2^k = 0 }
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    unsigned long long pow2 = 2;          /* 2^k , starts with k = 1 */
    for (idx_t k = 1; ; ++k) {
        unsigned long long limit = (unsigned long long)n + k;
        if (pow2 > limit)                /* 2^k already exceeds (n + k) */
            break;
        /* compute (n + k) & (2^k – 1); the operation is valid for all
           seq_t types because (2^k – 1) fits in 64 bits and is cast to seq_t. */
        seq_t term   = (seq_t)n + (seq_t)k;
        seq_t mask   = (seq_t)(pow2 - 1);
        if ((term & mask) == 0)
            ++count;
        pow2 <<= 1;                      /* next power of two */
        if (pow2 == 0)                   /* possible unsigned overflow */
            break;
    }
    return count;
}
