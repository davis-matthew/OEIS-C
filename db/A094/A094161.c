/*
 * A094161  Column 5 of A048790.
 * https://oeis.org/A094161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(n-1) + 305*C(n-1,2) + 1740*C(n-1,3) + 2000*C(n-1,4)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 86, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 according to the OEIS offset.  The function will work
       safely for n == 0 as well, returning 0. */
    seq_t s = (seq_t)(n - 1u);          /* s = n-1 */
    seq_t res = 5 * s;                  /* first term 5*(n-1) */

    if (n > 1) {
        seq_t c2 = s * (s - 1u) / 2;   /* C(s,2) */
        res += 305 * c2;
    }
    if (n > 2) {
        seq_t c3 = s * (s - 1u) * (s - 2u) / 6; /* C(s,3) */
        res += 1740 * c3;
    }
    if (n > 3) {
        seq_t c4 = s * (s - 1u) * (s - 2u) * (s - 3u) / 24; /* C(s,4) */
        res += 2000 * c4;
    }
    return res;
}
