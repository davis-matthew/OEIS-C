/*
 * A152845  Triangle read by rows, A007318 rows repeated seven times .
 * https://oeis.org/A152845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 1337, u32 n <= 4425, u64 n <= 8924,
 *            u128 n <= 8924, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A152845:
   Each row of Pascal's triangle (A007318) is listed 7 times consecutively.
   The sequence is the concatenation of these rows.
   For an index n, let t be the smallest positive integer such that
   7*t*(t+1)/2 > n.  Then r = t-1 is the row number, M = t is the row
   length, and pos = (n - 7*(t-1)*t/2) mod M.  The value is binomial(r,pos). */

static seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    return res;
}

seq_t a(idx_t n)
{
    /* Find smallest t such that 7*t*(t+1)/2 > n */
    seq_t t = 1;
    while (1) {
        seq_t S = (seq_t)7 * t * (t + 1) / 2;
        if (S > n) break;
        ++t;
    }

    idx_t r   = (idx_t)(t - 1);   /* row number */
    seq_t M   = t;                /* row length = r+1 */
    seq_t prevS = (seq_t)7 * (t - 1) * (t) / 2; /* S(r-1) */
    seq_t offset = n - prevS;
    idx_t pos = (idx_t)(offset % M);             /* position in the row */

    return binom_calc(r, pos);
}
