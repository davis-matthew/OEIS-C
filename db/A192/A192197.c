/*
 * A192197  Monotonic ordering of nonnegative differences 5^i-8^j, for 40>=
 *          i>=0, j>=0.
 * https://oeis.org/A192197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th smallest non‑negative value of 5^i-8^j with
 *            0≤i≤40, j≥0, i.e. the monotonic ordering of all such differences
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* size_t */

static seq_t vals[800];
static size_t val_cnt = 0;
static int initialized = 0;

static void init_seq(void)
{
    if (initialized) return;
    const idx_t max_i = 40;
    seq_t pow5 = 1;          /* 5^i for current i   */
    seq_t prev_pow5 = 0;      /* previous pow5 for overflow check */

    for (idx_t i = 0; i <= max_i; ++i)
    {
        if (i == 0) {
            pow5 = 1;
        } else {
            prev_pow5 = pow5;
            pow5 = prev_pow5 * 5;
            if (pow5 < prev_pow5) break;      /* overflow of pow5 */
        }

        seq_t pow8 = 1;          /* 8^j for current j   */
        seq_t prev_pow8 = 0;      /* previous pow8 for overflow check */
        while (1)
        {
            if (pow8 > pow5) break;              /* 8^j would be too large */

            /* store the non‑negative difference */
            vals[val_cnt++] = pow5 - pow8;

            /* advance pow8 */
            prev_pow8 = pow8;
            pow8 = prev_pow8 * 8;
            if (pow8 < prev_pow8) break;          /* overflow of pow8 */
        }
    }

    /* insertion sort ----------------------------------------------------- */
    for (size_t i = 1; i < val_cnt; ++i)
    {
        seq_t key = vals[i];
        size_t j = i;
        while (j > 0 && vals[j - 1] > key)
        {
            vals[j] = vals[j - 1];
            --j;
        }
        vals[j] = key;
    }

    /* remove duplicates ---------------------------------------------------- */
    size_t uniq = 0;
    for (size_t i = 0; i < val_cnt; ++i)
    {
        if (i == 0 || vals[i] != vals[i - 1])
            vals[uniq++] = vals[i];
    }
    val_cnt = uniq;

    initialized = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset – not used */
    init_seq();
    if (n <= val_cnt) return vals[n - 1];
    return 0;                       /* out of range */
}
