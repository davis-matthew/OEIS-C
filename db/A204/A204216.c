/*
 * A204216  Number of length 8 nonnegative integer arrays starting and ending
 *          with 0 with adjacent elements differing by no more than n.
 * https://oeis.org/A204216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (841*n^6 + 3636*n^5 + 6625*n^4 + 6570*n^3 + 3784*n^2 +
 *            1224*n + 180) / 180
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 30, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
#ifdef __SIZEOF_INT128__
    unsigned __int128 nn = n;
    unsigned __int128 n2 = nn * nn;
    unsigned __int128 n3 = n2 * nn;
    unsigned __int128 n4 = n3 * nn;
    unsigned __int128 n5 = n4 * nn;
    unsigned __int128 n6 = n5 * nn;
    unsigned __int128 num = 841 * n6
                         + 3636 * n5
                         + 6625 * n4
                         + 6570 * n3
                         + 3784 * n2
                         + 1224 * nn
                         + 180;
    return (seq_t)(num / 180);
#else
    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;
    seq_t n3  = n2 * sn;
    seq_t n4  = n3 * sn;
    seq_t n5  = n4 * sn;
    seq_t n6  = n5 * sn;
    seq_t num = 841 * n6;
    num += 3636 * n5;
    num += 6625 * n4;
    num += 6570 * n3;
    num += 3784 * n2;
    num += 1224 * sn;
    num += 180;
    return num / 180;
#endif
}
