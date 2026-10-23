/*
 * A164281  Triangle read by rows, a Petoukhov sequence (cf. A164279)
 *          generated from (1,2).
 * https://oeis.org/A164281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 2^popcount( Gray(n - (2^⌊log2(n+1)⌋ - 1)) ) where Gray(x)
 *            = x ^ (x >> 1) and ⌊log2(m)⌋ = 63 - __builtin_clzll(m) for m>0
 * Range:     u8 n <= 424, u16 n <= 1022, u32 n <= 1022, u64 n <= 1022,
 *            u128 n <= 1022, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the row r = floor(log2(n+1)) */
    idx_t m = n + 1;
    int r = 63 - __builtin_clzll(m);              /* m>0 because n>=0 */

    /* Offset where row r starts: 2^r - 1 */
    idx_t offset = ((idx_t)1 << r) - 1;

    /* Position within the row */
    idx_t k = n - offset;

    /* Gray code of k */
    unsigned long long gray = ((unsigned long long)k) ^ (((unsigned long long)k) >> 1);

    /* Number of 1-bits in the Gray code */
    unsigned int popcnt = __builtin_popcountll(gray);

    /* 2^popcnt fits in a uint64_t for n up to 10000 (row ∼ 14) */
    unsigned long long val = 1ULL << popcnt;

    return (seq_t)val;
}
