/*
 * A026039  a(n) = (d(n) - r(n))/5, where d = A026037 and r is the periodic
 *          sequence with fundamental period (1,2,0,2,0).
 * https://oeis.org/A026039
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 - 3*n^2 + 13*n)/30)
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 2580, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Behaviour for indices below the official offset is irrelevant. */
    if (n < 3) return 0;

    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;
    seq_t n3  = n2 * sn;
    seq_t num = (seq_t)2 * n3 - (seq_t)3 * n2 + (seq_t)13 * sn;
    return num / 30;
}
