/*
 * A086663  Maximum number of non-attacking knights on an n X n board with all
 *          interior squares removed.
 * https://oeis.org/A086663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n-3) for n>5; a(1)=1, a(2)=a(3)=4, a(4)=8, a(5)=12
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1)        return 1;
    if (n <= 3)        return 4;
    if (n == 4)        return 8;
    if (n == 5)        return 12;
    /* For n >= 6: a(n) = 4*(n-3) */
    return (seq_t)4 * ((seq_t)n - 3);
}
