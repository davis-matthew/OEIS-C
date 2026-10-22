/*
 * A001603  Odd-indexed terms of A124296.
 * https://oeis.org/A001603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 33*a(n-2) + 33*a(n-3) - 11*a(n-4) + a(n-5),
 *            with a(0)=1, a(1)=11, a(2)=101, a(3)=781, a(4)=5611
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static seq_t base[5] = {1, 11, 101, 781, 5611};
    
    if (n < 5) return base[n];
    
    seq_t a0 = base[0], a1 = base[1], a2 = base[2], a3 = base[3], a4 = base[4];
    seq_t an;
    
    for (idx_t i = 5; i <= n; i++) {
        an = (seq_t)11 * a4 - (seq_t)33 * a3 + (seq_t)33 * a2 - (seq_t)11 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    
    return a4;
}
