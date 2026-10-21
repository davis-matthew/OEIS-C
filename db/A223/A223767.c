/*
 * A223767  Number of nX5 0..1 arrays with rows, columns and antidiagonals
 *          unimodal and diagonals nondecreasing.
 * https://oeis.org/A223767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3628800)*n^10 - (1/241920)*n^9 + (23/120960)*n^8 -
 *            (43/40320)*n^7 + (11293/172800)*n^6 - (1721/2304)*n^5 +
 *            (1557823/181440)*n^4 - (3058823/60480)*n^3 + (99347/350)*n^2 -
 *            (196177/210)*n + 1664 for n>6
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 41, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    switch(n) {
        case 1: return (seq_t)16;
        case 2: return (seq_t)101;
        case 3: return (seq_t)317;
        case 4: return (seq_t)782;
        case 5: return (seq_t)1748;
        case 6: return (seq_t)3699;
    }
    __int128 x = (__int128)n;
    __int128 p = 1;
    p = p * x - 15;
    p = p * x + 690;
    p = p * x - 3870;
    p = p * x + 237153;
    p = p * x - 2710575;
    p = p * x + 31156460;
    p = p * x - 183529380;
    p = p * x + 1030029696;
    p = p * x - 3389938560;
    p = p * x + 6038323200;
    p /= 3628800;
    return (seq_t)p;
}
