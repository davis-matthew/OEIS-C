/*
 * A095369  Number of walks of length n between two nodes at distance 4 in the
 *          cycle graph C_9.
 * https://oeis.org/A095369
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 5·a(n-2) – 4·a(n-3) – 5·a(n-4) + 2·a(n-5) (n ≥
 *            9) Initial values: a(4) = 1, a(5) = 1, a(6) = 6, a(7) = 7, a(8)
 *            = 28
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t seq[20001];          /* space for all terms up to n = 20000   */
static idx_t memo_done = 3;       /* highest index that has been computed*/
static int initialized = 0;       /* 0 = not yet initialized            */

static void init_sequence(void)
{
    seq[4] = (seq_t)1;
    seq[5] = (seq_t)1;
    seq[6] = (seq_t)6;
    seq[7] = (seq_t)7;
    seq[8] = (seq_t)28;
    memo_done = 8;
    initialized = 1;
}

/* Compute a(n); n ≥ 4. For n < 4 the function returns 0. */
seq_t a(idx_t n)
{
    if (!initialized) init_sequence();

    if (n < 4) return (seq_t)0;

    if (n > memo_done) {
        for (idx_t i = memo_done + 1; i <= n; ++i) {
            seq[i] = seq[i-1]
                 + (seq_t)5 * seq[i-2]
                 + (seq_t)2 * seq[i-5]
                 - ( seq[i-3] * (seq_t)4 + seq[i-4] * (seq_t)5 );
        }
        memo_done = n;
    }
    return seq[n];
}
