/*
 * A190775  [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(1/2),3,2) and [ ]=floor.
 * https://oeis.org/A190775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((3*n+2)/sqrt(2)) - 3*floor(n/sqrt(2)) - 1
 * Range:     u8 n <= 4, u16 n <= 79, u32 n <= 132, u64 n <= 132,
 *            u128 n <= 132, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t floor_div_sqrt2(seq_t k) {
    seq_t kk = k * k;           /* k^2 */
    seq_t low = 0;
    seq_t high = k;             /* floor(k/√2) <= k */
    seq_t ans = 0;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        seq_t val = 2 * mid * mid;   /* 2*mid^2 */
        if (val <= kk) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;                       /* cast to seq_t */
    seq_t term1 = floor_div_sqrt2(3 * ns + 2); /* floor((3n+2)/√2) */
    seq_t term2 = floor_div_sqrt2(ns);         /* floor(n/√2) */
    return term1 - 3 * term2 - 1;              /* a(n) = term1 - 3*term2 - 1 */
}
