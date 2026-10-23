/*
 * A008340  Coordination sequence for E_8 lattice.
 * https://oeis.org/A008340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = if n = 0 then 1 else (456/7)*n^7 - 120*n^6 + 312*n^5 -
 *            120*n^4 - 48*n^3 + 240*n^2 -(624/7)*n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 13, u64 n <= 311, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;
    seq_t n3  = n2 * sn;
    seq_t n4  = n3 * sn;
    seq_t n5  = n4 * sn;
    seq_t n6  = n5 * sn;
    seq_t n7  = n6 * sn;

    seq_t pos1 = (seq_t)65 * n7;                /* 65 * n^7 */
    seq_t pos2 = (n7 - sn) / (seq_t)7;         /* (n^7 - n) / 7 */
    seq_t pos3 = (seq_t)312 * n5;              /* 312 * n^5 */
    seq_t pos4 = (seq_t)240 * n2;              /* 240 * n^2 */
    seq_t sum_pos = pos1 + pos2 + pos3 + pos4;

    seq_t neg1 = (seq_t)120 * n6;              /* 120 * n^6 */
    seq_t neg2 = (seq_t)120 * n4;              /* 120 * n^4 */
    seq_t neg3 = (seq_t)48  * n3;              /* 48  * n^3 */
    seq_t neg4 = (seq_t)89  * sn;              /* 89  * n */
    seq_t sum_neg = neg1 + neg2 + neg3 + neg4;

    return sum_pos - sum_neg;
}
