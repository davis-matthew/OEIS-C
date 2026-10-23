/*
 * A018544  Divisors of 604.
 * https://oeis.org/A018544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 604: a(n) = the nth divisor of 604 in increasing order */
seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t divs[6];

    if (!initialized) {
        int cnt = 0;
        for (int i = 0; i <= 2; ++i) {            /* 2^i where i = 0..2 */
            seq_t twoi = ((seq_t)1) << i;
            for (int j = 0; j <= 1; ++j) {        /* 151^j where j = 0..1 */
                seq_t val = twoi * ((j ? ((seq_t)151) : ((seq_t)1)));
                divs[cnt++] = val;
            }
        }
        /* simple bubble sort (6 elements only) */
        for (int k = 0; k < 5; ++k) {
            for (int l = 5; l > k; --l) {
                if (divs[l] < divs[l-1]) {
                    seq_t t = divs[l];
                    divs[l] = divs[l-1];
                    divs[l-1] = t;
                }
            }
        }
        initialized = 1;
    }

    if (n == 0 || n > (idx_t)6)
        return (seq_t)0;
    return divs[(int)(n - 1)];
}
