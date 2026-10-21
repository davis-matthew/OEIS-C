/*
 * A213831  Rectangular array: (row n) = b**c, where b(h) = 2*h-1, c(h) =
 *          3*n-5+3*h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A213831: a(n) = nth element of the rectangular array
 *  T(r,k) = convolution of b(h)=2h-1 and c(h)=3r-5+3h
 *  read by falling antidiagonals starting with a(1)=1.
 *  T(r,k) satisfies the 4-term recurrence
 *   T(r,k)=4*T(r,k-1)-6*T(r,k-2)+4*T(r,k-3)-T(r,k-4)
 *  with initial values
 *   T(r,0)=3r-2,  T(r,1)=12r-5,  T(r,2)=27r-3,  T(r,3)=48r+10.   */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, but such input is unlikely */

    /* Find the diagonal s such that T_{s-1} < n <= T_s, where
     * T_s = (s+1)(s+2)/2 is the number of elements up to that diagonal.
     * Since s is at most about sqrt(2n), a small linear search is sufficient. */
    idx_t s = 0;
    idx_t sum = 0;       /* T_{s-1} */
    while (sum + (s + 1) < n) {
        sum += s + 1;
        ++s;
    }
    /* Now sum = T_{s-1} and s satisfies: sum < n <= sum + (s+1) = T_s */

    idx_t prev_total = sum;                 /* T_{s-1} */
    idx_t offset     = n - prev_total;      /* 1 .. s+1 */
    idx_t r           = offset;             /* row number, 1..s+1 */
    idx_t k           = (s + 1) - r;        /* column index, 0..s */

    /* Convert to seq_t for arithmetic */
    seq_t R = (seq_t)r;

    /* Initial four values for row r */
    seq_t a0 = (seq_t)3 * R - (seq_t)2;        /* T(r,0) */
    seq_t a1 = (seq_t)12 * R - (seq_t)5;       /* T(r,1) */
    seq_t a2 = (seq_t)27 * R - (seq_t)3;       /* T(r,2) */
    seq_t a3 = (seq_t)48 * R + (seq_t)10;      /* T(r,3) */

    /* Return directly if k < 4 */
    if (k == 0) return a0;
    if (k == 1) return a1;
    if (k == 2) return a2;
    if (k == 3) return a3;

    /* Use recurrence for k >= 4 */
    seq_t prev4 = a0;
    seq_t prev3 = a1;
    seq_t prev2 = a2;
    seq_t prev1 = a3;
    for (idx_t i = 4; i <= k; ++i) {
        seq_t next = (seq_t)4 * prev1
                   - (seq_t)6 * prev2
                   + (seq_t)4 * prev3
                   - prev4;
        /* Shift the window of last four values */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;   /* T(r,k) */
}
