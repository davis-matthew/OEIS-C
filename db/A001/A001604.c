/*
 * A001604  Odd-indexed terms of A124297.
 * https://oeis.org/A001604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 33*a(n-2) + 33*a(n-3) - 11*a(n-4) + a(n-5)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t a0 = 11, a1 = 31, a2 = 151, a3 = 911, a4 = 5951;
    
    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;
    if (n == 4) return a4;
    
    for (idx_t i = 5; i <= n; i++) {
        seq_t a_new = 11 * a4 - 33 * a3 + 33 * a2 - 11 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a_new;
    }
    
    return a4;
}
