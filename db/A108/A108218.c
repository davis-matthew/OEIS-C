/*
 * A108218  Numbers of the form (11^i)*(12^j), with i, j >= 0.
 * https://oeis.org/A108218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = next triangular array of powers of 11 and 12 (no
 *            duplicates)
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 52, u64 n <= 185, u128 n <= 694,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;        /* array holding the sequence values */
static size_t dp_len = 0;       /* number of elements already computed */
static size_t dp_cap = 0;       /* allocated capacity of dp */
static size_t i11 = 0, i12 = 0; /* indices for generating next 11* and 12* values */

/* Ensure dp has at least 'need' capacity. */
static void ensure_capacity(size_t need)
{
    if (need <= dp_cap)
        return;
    size_t new_cap = dp_cap ? dp_cap << 1 : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* Allocation failed */
    dp = tmp;
    dp_cap = new_cap;
}

/* Generate terms until dp_len reaches 'n' */
static void generate_up_to(idx_t n)
{
    /* On first call, initialize the array with a(1)=1 */
    if (dp == NULL) {
        ensure_capacity(8);
        dp[0] = (seq_t)1;
        dp_len = 1;
        i11 = i12 = 0;
    }

    while (dp_len < (size_t)n) {
        seq_t next11 = dp[i11] * (seq_t)11;
        seq_t next12 = dp[i12] * (seq_t)12;
        seq_t val = next11 < next12 ? next11 : next12;

        ensure_capacity(dp_len + 1);
        dp[dp_len++] = val;

        if (val == next11)
            ++i11;
        if (val == next12)
            ++i12;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for n < 1, but return 0 to avoid UB */
    generate_up_to(n);
    return dp[(size_t)(n - 1)];
}
