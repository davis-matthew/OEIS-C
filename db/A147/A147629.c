/*
 * A147629  a(n) = Product_{k = 1..n-1} (9*k - 4).
 * https://oeis.org/A147629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k = 1..n-1} (9*k - 4) Initial value: a(1) = 1
 *            Recurrence: a(n+1) = (9*(n+1) - 4) * a(n) = (9*n + 5) * a(n)
 *            (for n >= 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) is defined to be 1; for n <= 1 the loop below is skipped */
    seq_t result = 1;
    for (idx_t k = 1; k < n; ++k) {
        /* Compute factor = 9*k - 4 avoiding overflow in idx_t */
        seq_t factor = ((seq_t)9 * k) - 4;
        result *= factor;
    }
    return result;
}
