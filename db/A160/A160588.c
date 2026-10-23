/*
 * A160588  Interleaving of A053645 and A000027.
 * https://oeis.org/A160588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (n%2==1 ? (n+1)/2 : ( (n/2+1) - 2^floor(log2(n/2+1)) ))
 * Range:     u8 n <= 290, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Interleaving of A053645 (remainder after highest power of 2) and A000027 (identity) */
seq_t a(idx_t n)
{
    if (n & 1) {                     /* odd index: a(2*k+1) = k+1 */
        return (seq_t)(n + 1) / 2;
    } else {                         /* even index: a(2*k) = A053645(k+1) */
        idx_t k = n / 2 + 1;         /* choose index for A053645 */
        idx_t p = 1;                 /* highest power of 2 <= k */
        while (p <= k / 2) {
            p <<= 1;
        }
        return (seq_t)(k - p);
    }
}
