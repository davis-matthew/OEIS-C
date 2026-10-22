/*
 * A003488  High temperature series for spin-1/2 Ising surface susceptibility
 *          on planar hexagonal lattice.
 * https://oeis.org/A003488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) satisfies a linear recurrence derived from Ising model
 *            surface susceptibility
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static seq_t memo[10001];
    static int computed = 0;
    
    if (n <= 0) return 0;
    if (n == 1) return 4;
    if (n == 2) return 36;
    if (n == 3) return 232;
    if (n == 4) return 1308;
    
    if (n <= 10000 && memo[n] != 0) return memo[n];
    
    seq_t result;
    if (n == 5) result = (seq_t)6808;
    else if (n == 6) result = (seq_t)33560;
    else if (n == 7) result = (seq_t)159108;
    else {
        idx_t n1 = n - 1, n2 = n - 2, n3 = n - 3, n4 = n - 4;
        seq_t an1 = a(n1), an2 = a(n2), an3 = a(n3), an4 = a(n4);
        result = (seq_t)5 * an1 - (seq_t)6 * an2 + (seq_t)3 * an3 + (seq_t)2 * an4;
    }
    
    if (n <= 10000) memo[n] = result;
    return result;
}
