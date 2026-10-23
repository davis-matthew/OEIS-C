/*
 * A037094  "Sloping binary representation" of Lucas numbers (A000032), slope
 *          = +1.
 * https://oeis.org/A037094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i >= 0} bit_i(Lucas(n+i), i) * 2^i
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where Lucas(0)=2, Lucas(1)=1, Lucas(k)=Lucas(k-1)+Lucas(k-2) */

seq_t a(idx_t n)
{
    /* Upper bound for i derived from i ≤ (log2(φ)/(1‑log2(φ)))·n ≈ 2.27·n,
       3 · n+5 is a safe over‑estimate. */
    const idx_t i_max     = 3 * n + 5;
    const idx_t max_k     = n + i_max;          /* last Lucas index to generate */
    const idx_t bits_max  = 8 * sizeof(seq_t);  /* number of defined bits in seq_t */

    seq_t result = 0;

    /* Initial two Lucas numbers */
    seq_t prev2 = (seq_t)2;   /* Lucas(0) */
    seq_t prev1 = (seq_t)1;   /* Lucas(1) */

    /* Generate Lucas numbers L_j for j = 0 … max_k and accumulate bits. */
    for (idx_t j = 0; j <= max_k; ++j) {
        seq_t cur;
        if (j == 0)                   /* L_0 */
            cur = prev2;
        else if (j == 1)              /* L_1 */
            cur = prev1;
        else {                         /* j ≥ 2: compute next Lucas */
            seq_t next = prev2 + prev1;
            prev2 = prev1;
            prev1 = next;
            cur = next;
        }

        if (j >= n) {               /* j corresponds to Lucas(n + i) with i = j‑n */
            const idx_t i = j - n;
            if (i > i_max || i >= bits_max)   /* no more relevant bits */
                break;

            seq_t bit = (cur >> i) & 1;
            if (bit)
                result |= ((seq_t)1 << i);
        }
    }

    return result;
}
