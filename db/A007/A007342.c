/*
 * A007342  Erroneous version of A001353.
 * https://oeis.org/A007342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 3*a(n-1) + 12*a(n-2) - 37*a(n-3) for n >= 5, with a(1)=1,
 *            a(2)=4, a(3)=15, a(4)=59
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 6, i64 n <= 6, i128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 4;
    if (n == 3) return 15;
    if (n == 4) return 59;
    
    seq_t prev3 = 4;
    seq_t prev2 = 15;
    seq_t prev1 = 59;
    
    for (idx_t i = 5; i <= n; i++) {
        seq_t curr = 3 * prev1 + 12 * prev2 - 37 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
