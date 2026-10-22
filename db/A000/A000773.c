/*
 * A000773  Number of numbers == 0 (mod 3) in range 2^n to 2^(n+1) with odd
 *          number of 1's in binary expansion.
 * https://oeis.org/A000773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 5*a(n-2) - 3*a(n-3) - 6*a(n-4) for n > 4; a(1) =
 *            a(2) = a(3) = 0, a(4) = 1
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 3) return 0;
    if (n == 4) return 1;
    
    seq_t a1 = 0, a2 = 0, a3 = 0, a4 = 1;
    
    for (idx_t i = 5; i <= n; i++) {
        seq_t next = a4 + 5*a3 - 3*a2 - 6*a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    
    return a4;
}
