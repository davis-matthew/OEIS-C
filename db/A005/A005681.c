/*
 * A005681  A squarefree quaternary sequence.
 * https://oeis.org/A005681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s(T(n-1), T(n)) where T is Thue-Morse, s(1,1)=1,
 *            s(1,2)=2, s(2,1)=3, s(2,2)=4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    int t1 = (__builtin_popcountll(n - 1) & 1) + 1;
    int t2 = (__builtin_popcountll(n) & 1) + 1;
    return (seq_t)(2 * t1 + t2 - 2);
}
