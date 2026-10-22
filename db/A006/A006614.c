/*
 * A006614  A variant of Zarankiewicz's problem: a(n) is the least k such that
 *          every n X n {0,1}-matrix with k ones contains an all-ones 2 X 4
 *          submatrix.
 * https://oeis.org/A006614
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) computed from periodic pattern in first differences with
 *            period 3
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 4) return 0;
    
    seq_t m = (seq_t)(n - 4);
    seq_t k = m / 3;
    seq_t r = m % 3;
    
    seq_t sum = 3*k*k + 15*k;
    
    if (r >= 1) sum += 7 + 2*k;
    if (r >= 2) sum += 5 + 2*k;
    
    return 14 + sum;
}
