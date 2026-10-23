/*
 * A192147  Monotonic ordering of nonnegative differences 3^i-4^j, for 40>=
 *          i>=0, j>=0.
 * https://oeis.org/A192147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 27, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generate the sequence of monotonic non‑negative differences
   3^i - 4^j  (0 ≤ i ≤ 40,  j ≥ 0).
   The values are first generated, sorted, duplicates removed, and then
   returned in ascending order.  */

/* Standard headers */
#include <stdlib.h>
#include <stdbool.h>

/* Comparison function for qsort on seq_t values. */
static int seq_cmp(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return  1;
    return 0;
}

/* Maximum indices specified in the sequence definition. */
#define MAX_I 40          /* 0 ≤ i ≤ 40 */
#define MAX_J 31          /* sufficient for 4^j ≤ 3^40 */

/* Static buffers for computed data. */
static seq_t seq_arr[2000];   /* enough for the expected ~700 values */
static idx_t seq_len = 0;
static bool seq_built = false;

/* Main sequence routine. */
seq_t a(idx_t n)
{
    /* Build the sequence lazily on first call. */
    if (!seq_built)
    {
        /* Pre‑compute powers of 3 and 4. */
        seq_t pow3[MAX_I + 1];
        seq_t pow4[MAX_J + 1];

        pow3[0] = (seq_t)1;
        for (int i = 1; i <= MAX_I; ++i)
            pow3[i] = pow3[i - 1] * (seq_t)3;

        pow4[0] = (seq_t)1;
        for (int j = 1; j <= MAX_J; ++j)
            pow4[j] = pow4[j - 1] * (seq_t)4;

        /* Temporary storage for raw differences. */
        seq_t raw[2000];
        idx_t raw_cnt = 0;

        for (int i = 0; i <= MAX_I; ++i)
        {
            for (int j = 0; j <= MAX_J; ++j)
            {
                if (pow4[j] > pow3[i])
                    break;               /* further j will be larger */
                raw[raw_cnt++] = pow3[i] - pow4[j];
            }
        }

        /* Sort the raw values. */
        qsort(raw, raw_cnt, sizeof(seq_t), seq_cmp);

        /* Remove duplicates in place. */
        idx_t uniq = 0;
        for (idx_t k = 0; k < raw_cnt; ++k)
        {
            if (uniq == 0 || raw[k] != raw[uniq - 1])
                raw[uniq++] = raw[k];
        }
        raw_cnt = uniq;

        /* Copy unique sorted values to the public array. */
        for (idx_t k = 0; k < raw_cnt; ++k)
            seq_arr[k] = raw[k];
        seq_len = raw_cnt;
        seq_built = true;
    }

    if (n == 0 || n > seq_len)
        return (seq_t)0;          /* undefined for out‑of‑range indices */
    return seq_arr[n - 1];
}
