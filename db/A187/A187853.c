/*
 * A187853  Number of 5-step king-knight's tours (piece capable of both kinds
 *          of moves) on an n X n board summed over all starting positions.
 * https://oeis.org/A187853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) for n>10; initial values:
 *            a(1)=0, a(2)=0, a(3)=5328, a(4)=49776, a(5)=177040, a(6)=408048,
 *            a(7)=744696, a(8)=1183632, a(9)=1723120, a(10)=2362864
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 10) {
        static const seq_t init[10] = {
            0ULL, 0ULL, 5328ULL, 49776ULL, 177040ULL, 408048ULL,
            744696ULL, 1183632ULL, 1723120ULL, 2362864ULL
        };
        return init[n - 1];
    }
    seq_t a_nm3 = 1183632ULL;   /* a(8) */
    seq_t a_nm2 = 1723120ULL;   /* a(9) */
    seq_t a_nm1 = 2362864ULL;   /* a(10) */
    for (idx_t i = 11; i <= n; ++i) {
        seq_t new_val = 3 * a_nm1 - 3 * a_nm2 + a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = new_val;
    }
    return a_nm1;
}
