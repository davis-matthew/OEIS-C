/*
 * A062076  a(n) = (2n-1)^n * n^(2n-1).
 * https://oeis.org/A062076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (2n-1)^n * n^(2n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: fast modular exponentiation for arbitrary precision or 128‑bit
   integers. Works for all sizes of seq_t; no external dependencies. */
static inline seq_t pow_seq_local(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1) result = result * base;
        base = base * base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute (2n-1)^n */
    seq_t base1 = (seq_t)((idx_t)2 * n - (idx_t)1);
    seq_t part1 = pow_seq_local(base1, n);

    /* Compute n^(2n-1) */
    seq_t base2 = (seq_t)n;
    idx_t exp2  = (idx_t)2 * n - (idx_t)1;
    seq_t part2 = pow_seq_local(base2, exp2);

    return part1 * part2;
}
