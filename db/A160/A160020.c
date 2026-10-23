/*
 * A160020  Row sums of triangle in A160019 .
 * https://oeis.org/A160020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = b^2 + (n+1-b)*(n-b), where b = 2^popcount(n)
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Popcount of n (number of 1 bits) */
    unsigned popcnt = __builtin_popcountll((unsigned long long)n);

    /* Compute b = 2^popcnt */
    seq_t b = (seq_t)1 << popcnt;

    /* Convert n to seq_t for arithmetic */
    seq_t nb = (seq_t)n;

    /* Compute the product term without signed arithmetic */
    if (b <= nb) {
        seq_t k = nb - b;          /* k = n - b  (>= 0) */
        seq_t prod = k * (k + 1);   /* k*(k+1)   (>= 0) */
        return b * b + prod;        /* b^2 + k*(k+1) */
    } else {
        seq_t k = b - nb;          /* k = b - n  (> 0) */
        seq_t prod = k * (k - 1);   /* k*(k-1)   (>= 0) */
        return b * b + prod;        /* b^2 + k*(k-1) */
    }
}
