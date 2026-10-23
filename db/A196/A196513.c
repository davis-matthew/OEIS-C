/*
 * A196513  a(n) = 1*4*7 + 4*7*10 + 7*10*13 + ... (n terms).
 * https://oeis.org/A196513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (3*n + 5) * (9*n*n + 15*n - 10) / 4
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 157, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(n) = a(n-1) + (3*n-2)*(3*n+1)*(3*n+4) with a(0)=0 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t term = ((seq_t)(3 * i - 2)) *
                     ((seq_t)(3 * i + 1)) *
                     ((seq_t)(3 * i + 4));
        sum += term;
    }
    return sum;
}
