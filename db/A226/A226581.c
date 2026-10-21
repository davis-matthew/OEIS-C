/*
 * A226581  Smallest number of integer-sided squares needed to tile an 8 X n
 *          rectangle.
 * https://oeis.org/A226581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A226581 – Smallest number of integer‑sided squares needed to tile an
    8×n rectangle.
    Recurrence:
        a(n) = a(n-8) + 1            for n > 9
    Initial values:
        a(0)=0, a(1)=8, a(2)=4, a(3)=5, a(4)=2,
        a(5)=5, a(6)=4, a(7)=7, a(8)=1, a(9)=7
    The implementation reduces n modulo 8 by applying
    the recurrence k times, where k=ceil((n-9)/8). */

seq_t a(idx_t n)
{
    static const seq_t init[10] = {0, 8, 4, 5, 2, 5, 4, 7, 1, 7};

    if (n <= 9)
        return init[n];

    /* k = ceil((n-9)/8) = (n-9+7)/8 */
    idx_t k = (n - 9 + 7) / 8;
    idx_t m = n - 8 * k;           /* 0 <= m <= 9 */

    return init[m] + (seq_t)k;
}
