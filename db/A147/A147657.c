/*
 * A147657  a(1)=1, a(2)=2, thereafter (1, -2, 3, -4, 5, -6, ...) interleaved
 *          with (-2, 2, -2, 2, ...).
 * https://oeis.org/A147657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(1)=1, a(2)=2; for n≥3, let k=(n-3)/4 and r=(n-3)%4. Then r==0
 *            → a(n)=2*k+1 r==1 → a(n)=-2 r==2 → a(n)=-(2*k+2) r==3 → a(n)=2
 * Range:     i8 n <= 258, i16 n <= 5002, i32 n <= 5002, i64 n <= 5002,
 *            i128 n <= 5002, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    idx_t k = (n - 3) / 4;
    idx_t r = (n - 3) % 4;
    switch (r) {
        case 0: return (seq_t)(2 * (seq_t)k + 1);
        case 1: return (seq_t)(-2);
        case 2: return (seq_t)(-(2 * (seq_t)k + 2));
        case 3: return (seq_t)2;
    }
    /* unreachable */
    return (seq_t)0;
}
