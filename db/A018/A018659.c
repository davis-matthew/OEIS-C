/*
 * A018659  Divisors of 810.
 * https://oeis.org/A018659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 810 in ascending order */

static seq_t divisors[20];
static int init = 0;

static void precompute(void)
{
    seq_t num = (seq_t)810;
    idx_t lim = 0;
    /* find integer sqrt of 810 */
    while ((lim + 1) * (lim + 1) <= (idx_t)num)
        ++lim;
    idx_t cnt = 0;
    for (idx_t i = 1; i <= lim; ++i) {
        if (num % (seq_t)i == 0) {
            divisors[cnt++] = (seq_t)i;
            seq_t other = num / (seq_t)i;
            if (i != (idx_t)other)
                divisors[cnt++] = other;
        }
    }
    /* sort the list ascending */
    for (idx_t i = 0; i < cnt - 1; ++i)
        for (idx_t j = i + 1; j < cnt; ++j)
            if (divisors[i] > divisors[j]) {
                seq_t tmp = divisors[i];
                divisors[i] = divisors[j];
                divisors[j] = tmp;
            }
    init = 1;
}

seq_t a(idx_t n)
{
    if (!init)
        precompute();
    if (n == 0 || n > 20)
        return (seq_t)0;
    return divisors[n - 1];
}
