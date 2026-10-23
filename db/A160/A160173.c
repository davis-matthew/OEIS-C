/*
 * A160173  Number of T-toothpicks added at n-th stage to the T-toothpick
 *          structure of A160172.
 * https://oeis.org/A160173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2/3)*(3^wt(n-1)+3^wt(n-2)) + 1 (n >= 3) Initial values:
 *            a(0) = 0, a(1) = 1, a(2) = 3
 * Range:     u8 n <= 31, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow3[65] = {0};   /* powers of 3^i for i = 0..64 */

static int popcount_u64(idx_t x)
{
    int c = 0;
    while (x)
    {
        x &= x - 1;
        ++c;
    }
    return c;
}

static void init_pow3(void)
{
    static int done = 0;
    if (done) return;
    pow3[0] = 1;
    for (int i = 1; i <= 64; ++i)
        pow3[i] = pow3[i - 1] * 3;
    done = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 3;

    init_pow3();

    int pc1 = popcount_u64(n - 1);
    int pc2 = popcount_u64(n - 2);

    seq_t sum = pow3[pc1] + pow3[pc2];
    sum = (sum * 2) / 3;   /* integer division is exact */
    return sum + 1;
}
