/*
 * A227365  a(n) = 0 + 1*2 + 3*4*5 + 6*7*8*9 + ... + ...*n.
 * https://oeis.org/A227365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σₖ₌₀^{⌊(√(8n+1)-1)/2⌋} Π_{i=k(k+1)/2}^{min(n, k(k+1)/2 +
 *            k)} i
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 64, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0;; ++k) {
        idx_t start = k * (k + 1) / 2;          /* first index of block k */
        if (start > n) break;
        idx_t end   = start + k;                /* last index of full block k */
        idx_t to    = (n < end) ? n : end;      /* last index we actually use */
        seq_t prod  = 1;
        for (idx_t i = start; i <= to; ++i) {
            prod *= (seq_t)i;
        }
        sum += prod;
    }
    return sum;
}
