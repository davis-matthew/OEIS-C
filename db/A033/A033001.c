/*
 * A033001  Every run of digits of n in base 3 has length 2.
 * https://oeis.org/A033001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 4*A043307(n)
 * Range:     u8 n <= 6, u16 n <= 62, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    idx_t k = 1;           /* length of the pair sequence   */
    idx_t power = 2;       /* 2^k – number of sequences of that length */
    idx_t cum = power;     /* total number of sequences up to length k */

    while (cum < n) {
        k++;               /* extend length   */
        power <<= 1;       /* 2^k            */
        cum += power;      /* add new block  */
    }

    /* sequences of shorter lengths: */
    idx_t prev_sum   = cum - power;           /* Σ_{t=1}^{k-1} 2^t */
    idx_t idx_within = n - prev_sum - 1;      /* 0‑based index inside length‑k block */

    seq_t M = 0;                 /* will hold A043307(n) */
    /* first digit: bit (k‑1) of idx_within */
    int prev_digit = ((idx_within >> (k - 1)) & 1) ? 2 : 1;
    M = (seq_t)prev_digit;

    for (int i = 1; i < k; i++) {
        int bit = (idx_within >> (k - 1 - i)) & 1;
        int next_digit;
        switch (prev_digit) {
            case 0: next_digit = bit ? 2 : 1; break;
            case 1: next_digit = bit ? 2 : 0; break;
            default: /* prev_digit == 2 */
                     next_digit = bit ? 1 : 0; break;
        }
        M = M * (seq_t)9 + (seq_t)next_digit;
        prev_digit = next_digit;
    }

    return M * (seq_t)4;
}
