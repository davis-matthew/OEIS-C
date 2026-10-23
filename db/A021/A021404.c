/*
 * A021404  Expansion of 1/((1-x)*(1-3x)*(1-4x)*(1-12x)).
 * https://oeis.org/A021404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 115*a(n-2) + 240*a(n-3) - 144*a(n-4) initial
 *            values: a(0)=1, a(1)=20, a(2)=285, a(3)=3640
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)20;
    if (n == 2) return (seq_t)285;
    if (n == 3) return (seq_t)3640;

    seq_t a0 = (seq_t)1;        // a(n-4)
    seq_t a1 = (seq_t)20;       // a(n-3)
    seq_t a2 = (seq_t)285;      // a(n-2)
    seq_t a3 = (seq_t)3640;     // a(n-1)

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute: 20*a3 - 115*a2 + 240*a1 - 144*a0
           Do positive terms first to avoid unsigned underflow */
        seq_t pos = (seq_t)20 * a3 + (seq_t)240 * a1;
        seq_t neg = (seq_t)115 * a2 + (seq_t)144 * a0;
        seq_t next = pos - neg;        // pos >= neg for all n >= 4

        /* Shift indices */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
