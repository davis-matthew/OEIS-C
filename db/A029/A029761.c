/*
 * A029761  Partial sums of A005001.
 * https://oeis.org/A029761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^n (k+1) * Bell(n-k) (partial sums of A005001)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define MAX_N 20000

seq_t a(idx_t n)
{
    /* static data for memoization */
    static seq_t bell[MAX_N + 1];
    static seq_t cumbell[MAX_N + 1];
    static seq_t seq[MAX_N + 1];
    static seq_t row[2][MAX_N + 1];
    static int cur = 0, prev = 1;
    static int initialized = 0;
    static int last = -1;               /* largest n already computed */

    if (n > (idx_t)MAX_N)
        return 0;                       /* out of supported range */

    /* first‑time initialisation */
    if (!initialized) {
        bell[0]        = 1;              /* Bell(0)   */
        cumbell[0]     = 1;              /* S(0)      */
        seq[0]         = 1;              /* a(0)      */
        row[prev][0]   = 1;              /* row for n=0 */
        initialized    = 1;
        last           = 0;
    }

    /* extend precomputation up to n */
    for (int i = last + 1; i <= (int)n; ++i) {
        /* compute Bell(i) with Bell triangle */
        row[cur][0] = row[prev][i - 1];
        for (int j = 1; j <= i; ++j) {
            row[cur][j] = row[cur][j - 1] + row[prev][j - 1];
        }
        bell[i] = row[cur][0];

        cumbell[i] = cumbell[i - 1] + bell[i];
        seq[i]     = seq[i - 1] + cumbell[i];

        /* swap rows for next iteration */
        int tmp = cur;
        cur = prev;
        prev = tmp;
    }

    last = (int)n;
    return seq[n];
}
