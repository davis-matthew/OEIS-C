/*
 * A067061  A permutation of the natural numbers.
 * https://oeis.org/A067061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(6k-5)=6k-2; a(6k-4)=6k-5; a(6k-3)=6k-1; a(6k-2)=6k-4;
 *            a(6k-1)=6k; a(6k)=6k-3. (k ≥ 1)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    idx_t k = (n + 5) / 6;             /* block number (k ≥ 1)     */
    seq_t base = (seq_t)k * 6;         /* 6k, fits in seq_t        */
    idx_t offset = n - (6 * k - 5);    /* 0..5                      */
    switch (offset) {
        case 0: return base - 2;       /* a(6k-5) */
        case 1: return base - 5;       /* a(6k-4) */
        case 2: return base - 1;       /* a(6k-3) */
        case 3: return base - 4;       /* a(6k-2) */
        case 4: return base;           /* a(6k-1) */
        case 5: return base - 3;       /* a(6k)   */
    }
    return 0; /* unreachable */
}
