/*
 * A028067  Expansion of 1/((1-3x)(1-5x)(1-8x)(1-12x)).
 * https://oeis.org/A028067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5*12^(n+3)-21*8^(n+3)+30*5^(n+3)-14*3^(n+3))/1260
 *            Equivalent linear recurrence derived from the generating
 *            function: a(0)=1, a(1)=28, a(2)=513, a(3)=7844 a(n) = 28*a(n-1)
 *            - 271*a(n-2) + 1068*a(n-3) - 1440*a(n-4) (n≥4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)28;
    if (n == 2) return (seq_t)513;
    if (n == 3) return (seq_t)7844;

    seq_t a0 = (seq_t)1, a1 = (seq_t)28, a2 = (seq_t)513, a3 = (seq_t)7844;
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)28 * a3 - (seq_t)271 * a2 + (seq_t)1068 * a1 - (seq_t)1440 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
