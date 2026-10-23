/*
 * A164006  Zero together with row 6 of the array in A163280.
 * https://oeis.org/A164006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+5) for n > 4; initial values: a(0)=0, a(1)=11,
 *            a(2)=22, a(3)=27, a(4)=44
 * Range:     u8 n <= 13, u16 n <= 253, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first few terms explicitly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)22;
    if (n == 3) return (seq_t)27;
    if (n == 4) return (seq_t)44;
    /* For n >= 5, use the closed form */
    seq_t sn = (seq_t) n;
    return sn * (sn + (seq_t)5);
}
