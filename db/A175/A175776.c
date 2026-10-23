/*
 * A175776  Partial sums of floor(n^2/15).
 * https://oeis.org/A175776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-15) + (n+1)*(n-15) + 78 for n > 14 Initial values:
 *            a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=2, a(6)=4, a(7)=7,
 *            a(8)=11, a(9)=16, a(10)=22, a(11)=30, a(12)=39, a(13)=50,
 *            a(14)=63. The recurrence uses only non‑negative arithmetic so it
 *            works with unsigned seq_t. The function works fo...
 * Range:     u8 n <= 22, u16 n <= 142, u32 n <= 5781, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base values for n = 0 .. 14 */
    static const seq_t base[15] = {
        0, 0, 0, 0, 1,
        2, 4, 7, 11, 16,
        22, 30, 39, 50, 63
    };

    /* for small n use the table directly */
    if (n < 15) {
        return base[n];
    }

    /* iterative use of the recurrence to avoid recursion */
    idx_t r  = n % 15;          /* index into the base array */
    seq_t  res = base[r];

    idx_t m = r;                /* current index in the recurrence */
    while (m < n) {
        m += 15;

        /* term = (m+1) * (m-15) + 78  (all non‑negative) */
        seq_t term = ((seq_t)(m + 1)) * ((seq_t)(m - 15)) + (seq_t)78;
        res += term;
    }

    return res;
}
