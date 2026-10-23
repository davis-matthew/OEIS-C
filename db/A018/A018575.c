/*
 * A018575  Divisors of 656.
 * https://oeis.org/A018575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n‑th divisor of 656 in increasing order
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 656 = 2^4 * 41, so the divisors are: 1,2,4,8,16,41,82,164,328,656. */

seq_t a(idx_t n)
{
    static seq_t divs[10];
    static int init = 0;
    if (!init) {
        int i = 0;
        for (idx_t e = 0; e <= 4; ++e)      /* 2^e */
            divs[i++] = ((seq_t)1 << e);
        for (idx_t e = 0; e <= 4; ++e)      /* 41 * 2^e */
            divs[i++] = (((seq_t)1 << e) * ((seq_t)41));
        init = 1;
    }
    if (n < 1 || n > 10)          /* sequence has only 10 terms */
        return (seq_t)0;
    return divs[n-1];
}
