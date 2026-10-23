/*
 * A126368  Number of base 14 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Number of base 14 n‑digit words with adjacent digits differing
 *            by at most 1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  i.e. a(0)=1, a(1)=14, a(2)=40, … as in OEIS A126368. */

seq_t a(idx_t n)
{
    /* Handle the trivial cases */
    if (n == (idx_t)0) return (seq_t)1;
    const idx_t B = 14;                 /* digit alphabet size */
    seq_t cur[B];
    /* For length 1 every digit 0‑13 is allowed */
    for (idx_t d = 0; d < B; ++d)
        cur[d] = (seq_t)1;

    if (n == (idx_t)1) {
        seq_t tot = 0;
        for (idx_t d = 0; d < B; ++d) tot += cur[d];
        return tot;                     /* 14 */
    }

    seq_t next[B];
    /* Dynamic programming for lengths 2 … n */
    for (idx_t step = 2; step <= n; ++step) {
        for (idx_t d = 0; d < B; ++d) {
            seq_t s = cur[d];           /* same digit */
            if (d > 0)      s += cur[d-1];   /* one less */
            if (d+1 < B)    s += cur[d+1];   /* one more */
            next[d] = s;
        }
        /* copy next into cur for the next iteration */
        for (idx_t d = 0; d < B; ++d)
            cur[d] = next[d];
    }

    /* Sum all ending digits to obtain a(n) */
    seq_t tot = 0;
    for (idx_t d = 0; d < B; ++d) tot += cur[d];
    return tot;
}
