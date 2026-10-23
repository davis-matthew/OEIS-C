/*
 * A172221  Number of ways to place 3 nonattacking zebras on a 3 X n board.
 * https://oeis.org/A172221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^3 - 21*n^2 + 50*n - 48)/2 for n >= 6, with a(1)=1,
 *            a(2)=20, a(3)=84, a(4)=200, a(5)=403
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 873, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 1: return 1;
        case 2: return 20;
        case 3: return 84;
        case 4: return 200;
        case 5: return 403;
        default:
            seq_t nn = (seq_t)n;
            return (9*nn*nn*nn - 21*nn*nn + 50*nn - 48) / 2;
    }
}
