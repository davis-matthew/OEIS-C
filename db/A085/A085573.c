/*
 * A085573  2*Sum(floor(C(n,w)/w),w=1..n/2-1)+floor(C(n,n/2)/(n/2)) if n is
 *          even, otherwise 2*Sum(floor(C(n,w)/w),w=1..(n-1)/2).
 * https://oeis.org/A085573
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*∑_{w=1}^{⌊(n-1)/2⌋} ⌊C(n,w)/w⌋ + (n even ?
 *            ⌊C(n,n/2)/(n/2)⌋ : 0)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 30, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 2 (a(2) is the first term) */

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    idx_t wmax = (n % 2 == 0) ? (n / 2 - 1) : ((n - 1) / 2);
    seq_t sum = 0;
    seq_t bin = 1; /* C(n,0) */
    for (idx_t w = 1; w <= wmax; ++w) {
        /* Compute C(n,w) from C(n,w-1) */
        bin = bin * (seq_t)(n - w + 1) / (seq_t)w;
        /* Floor division by w */
        seq_t term = bin / (seq_t)w;
        sum += term;
    }
    seq_t result = sum * 2;
    if (n % 2 == 0) {
        idx_t midw = n / 2;
        /* Compute C(n,midw) from C(n,midw-1)=bin */
        seq_t binmid = bin * (seq_t)(midw + 1) / (seq_t)midw;
        seq_t mid = binmid / (seq_t)midw;
        result += mid;
    }
    return result;
}
