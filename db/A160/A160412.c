/*
 * A160412  Number of "ON" cells at n-th stage in simple 2-dimensional
 *          cellular automaton (see Comments for precise definition).
 * https://oeis.org/A160412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * A160410(n) / 4 where A160410(0) = 0 and for n > 0
 *            A160410(n) = 4^(⌊log2 n⌋+1) + 3 * A160410(n - 2^⌊log2 n⌋)
 * Range:     u8 n <= 7, u16 n <= 172, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute 4^exp for small exp. */
static seq_t pow4_int(int exp)
{
    seq_t r = 1;
    for (int i = 0; i < exp; ++i)
        r *= (seq_t)4;
    return r;
}

/* Recursive computation of A160410(n).  All arithmetic is done in seq_t. */
static seq_t A160410_recursive(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Highest set bit position of n (floor(log2 n)) */
    unsigned long long u = (unsigned long long)n;
    int L = 63 - __builtin_clzll(u);          /* floor(log2 n) */

    idx_t pow2 = (idx_t)1 << L;               /* 2^L */
    seq_t pow4 = pow4_int(L + 1);             /* 4^(L+1) = 2^(2L+2) */

    return pow4 + (seq_t)3 * A160410_recursive(n - pow2);
}

/* Public function that returns a(n). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* a(n) = 3 * A160410(n) / 4 */
    return (A160410_recursive(n) * (seq_t)3) / (seq_t)4;
}
