/*
 * A193409  Crater numbers.
 * https://oeis.org/A193409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 40, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the Crater numbers (OEIS A193409). 
   For each k = 1..10 (odd length 2*k-1), and for each c = 0..10-k,
   the number is formed by digits:
        d[i] = c + (k-1-i)   for i = 0..k-1
        d[2*k-2-i] = d[i]   for i = 0..k-2
   The sequence a(n) lists these numbers in increasing order of k,
   then increasing c.  The total number of terms is 55. */

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n<1, return 0 */

    /* Precomputed counts per group k: 10,9,8,...,1 */
    static const int count_per_k[10] = {10,9,8,7,6,5,4,3,2,1};

    // Find group k such that cumulative count >= n
    idx_t cum = 0;
    int k = 1;
    for ( ; k <= 10; ++k) {
        int cnt = count_per_k[k-1];
        if (n <= cum + (idx_t)cnt) {
            break;
        }
        cum += (idx_t)cnt;
    }
    if (k > 10) return 0;  /* n beyond defined terms */

    // Offset within group k (0-based)
    idx_t offset = n - cum - 1;
    // Center digit c
    int c = (int)offset;  /* offset fits in int because cnt <= 10 */

    seq_t result = 0;
    /* Left half including middle digit */
    for (int i = 0; i < k; ++i) {
        seq_t d = (seq_t)(c + (k - 1 - i));
        result = result * (seq_t)10 + d;
    }
    /* Right half (excluding middle digit) */
    for (int i = k - 2; i >= 0; --i) {
        seq_t d = (seq_t)(c + (k - 1 - i));
        result = result * (seq_t)10 + d;
    }
    return result;
}
