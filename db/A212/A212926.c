/*
 * A212926  Number of n X 4 0..3 arrays with no column j greater than column
 *          j-1 in all rows.
 * https://oeis.org/A212926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -3*96^n + 256^n + 36^n - 1 + 2*16^n (OEIS A212926)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    /* Compute each power once. */
    seq_t pow256 = pow_seq((seq_t)256, n);
    seq_t pow36  = pow_seq((seq_t)36,  n);
    seq_t pow16  = pow_seq((seq_t)16,  n);
    seq_t pow96  = pow_seq((seq_t)96,  n);

    /* Sum the positive terms. */
    seq_t sum_pos = pow256;
    sum_pos += pow36;
    sum_pos += pow16 * (seq_t)2;

    /* Sum the negative terms. */
    seq_t sum_neg = pow96 * (seq_t)3;
    sum_neg += (seq_t)1;

    /* Final result (the expression is non‑negative for n >= 1). */
    return sum_pos - sum_neg;
}
