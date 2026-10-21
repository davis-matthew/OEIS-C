/*
 * A209443  a(n) = Pell(n)*A000118(n) for n>=1 with a(0)=1, where A000118(n)
 *          is the number of ways of writing n as a sum of 4 squares.
 * https://oeis.org/A209443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Pell(n) * A000118(n) for n >= 1 with a(0) = 1 Pell(0)=0,
 *            Pell(1)=1, Pell(n)=2*Pell(n-1)+Pell(n-2) A000118(n)=8 *
 *            sum_{d|n, 4∤d} d
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 20, u64 n <= 44, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* ----- compute Pell(n) ----- */
    seq_t pell;
    if (n == 0) {
        pell = (seq_t)0;
    } else if (n == 1) {
        pell = (seq_t)1;
    } else {
        seq_t p0 = (seq_t)0;
        seq_t p1 = (seq_t)1;
        for (idx_t i = 2; i <= n; ++i) {
            seq_t p_next = (seq_t)2 * p1 + p0;
            p0 = p1;
            p1 = p_next;
        }
        pell = p1;
    }

    /* ----- compute A000118(n) = 8 * sum_{d|n, 4∤d} d ----- */
    seq_t sum_div = (seq_t)0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            if (d % 4 != 0) sum_div += (seq_t)d;
            idx_t e = n / d;
            if (e != d && e % 4 != 0) sum_div += (seq_t)e;
        }
    }

    seq_t result = pell * ( (seq_t)8 * sum_div );
    return result;
}
