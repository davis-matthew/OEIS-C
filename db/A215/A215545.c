/*
 * A215545  Number of standard Young tableaux of shape [5*n,5].
 * https://oeis.org/A215545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n-4)*(5*n+2)*(5*n+3)*(5*n+4)*(n+1)/24 (a(0)=0)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 22, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) {
        return 0;
    }

    /* Compute 5*n as a seq_t first to avoid overflow in intermediate steps. */
    seq_t five_n = (seq_t)n * 5;

    seq_t t1 = five_n - 4;
    seq_t t2 = five_n + 2;
    seq_t t3 = five_n + 3;
    seq_t t4 = five_n + 4;
    seq_t t5 = (seq_t)n + 1;

    seq_t prod = t1;
    prod = prod * t2;
    prod = prod * t3;
    prod = prod * t4;
    prod = prod * t5;

    /* The division by 24 is exact. */
    return prod / 24;
}
