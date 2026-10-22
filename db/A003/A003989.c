/*
 * A003989  Triangle T from the array A(x, y) = gcd(x,y), for x >= 1, y >= 1,
 *          read by antidiagonals.
 * https://oeis.org/A003989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look, nice, mult, tabl
 * Formula:   Triangle read by rows where T(n,k) = gcd(n-k+1, k) for 1 <= k <=
 *            n
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t gcd(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

seq_t a(idx_t n) {
    /* Map linear index n to (row, col) in triangular array */
    idx_t row = 1;
    while (row * (row + 1) / 2 < n) {
        row++;
    }
    
    idx_t col = n - row * (row - 1) / 2;
    
    /* Return T(row, col) = gcd(row - col + 1, col) */
    return gcd((seq_t)(row - col + 1), (seq_t)col);
}
