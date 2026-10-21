/*
 * A051442  a(n) = n^(n+1)+(n+1)^n.
 * https://oeis.org/A051442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^(n+1) + (n+1)^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n^(n+1) */
    seq_t n_pow   = 1;
    seq_t base_n  = (seq_t)n;
    idx_t exp_n   = n + 1;
    while (exp_n) {
        if (exp_n & 1) n_pow *= base_n;
        base_n *= base_n;
        exp_n >>= 1;
    }

    /* Compute (n+1)^n */
    seq_t np1_pow = 1;
    seq_t base_np1 = (seq_t)(n + 1);
    idx_t exp_np1 = n;
    while (exp_np1) {
        if (exp_np1 & 1) np1_pow *= base_np1;
        base_np1 *= base_np1;
        exp_np1 >>= 1;
    }

    return n_pow + np1_pow;
}
