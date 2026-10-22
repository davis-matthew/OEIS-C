/*
 * A000956  A sequence satisfying (a(2n+1) + 1)^3 = Sum_{k=1..2n+1} a(k)^3.
 * https://oeis.org/A000956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=17, a(3)=40, a(2n+2) = 3*a(2n+1)^2 + 8*a(2n+1) + 6,
 *            a(2n+3) = 3*a(2n+1)^3 + 12*a(2n+1)^2 + 17*a(2n+1) + 8
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static seq_t memo[10001];
    static char computed[10001];
    
    if (n <= 10000 && computed[n]) {
        return memo[n];
    }
    
    seq_t result;
    if (n == 1) result = 2;
    else if (n == 2) result = 17;
    else if (n == 3) result = 40;
    else if (n % 2 == 0) {
        seq_t prev = a(n - 1);
        result = 3 * prev * prev + 8 * prev + 6;
    } else {
        seq_t prev = a(n - 2);
        result = 3 * prev * prev * prev + 12 * prev * prev + 17 * prev + 8;
    }
    
    if (n <= 10000) {
        memo[n] = result;
        computed[n] = 1;
    }
    
    return result;
}
