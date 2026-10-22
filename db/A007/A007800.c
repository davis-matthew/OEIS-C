/*
 * A007800  From a problem in AI planning: a(n) =
 *          4+a(n-1)+a(n-2)+a(n-3)+a(n-4)-a(n-5)-a(n-6)-a(n-7), n>7.
 * https://oeis.org/A007800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-4) + a(n-5), with a(1)=1,
 *            a(2)=2, a(3)=4, a(4)=8, a(5)=16
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 73, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 4;
    if (n == 4) return 8;
    if (n == 5) return 16;
    
    // For n >= 6, use recurrence: a(n) = 3*a(n-1) - 2*a(n-2) - a(n-4) + a(n-5)
    seq_t a1 = 1, a2 = 2, a3 = 4, a4 = 8, a5 = 16;
    seq_t result;
    
    for (idx_t i = 6; i <= n; i++) {
        result = 3 * a5 - 2 * a4 - a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = result;
    }
    
    return result;
}
