/*
 * A115097  Powers of 10 seconds, expressed as days, hours, minutes, seconds.
 * https://oeis.org/A115097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 28, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A115097: Powers of 10 seconds expressed as days, hours, minutes, seconds. */
/* Each term is the next component in the concatenated list of
   10^k (seconds) broken into days, hours, minutes and seconds. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache of cumulative term counts and powers of ten */
    static idx_t *cum_counts = NULL;
    static size_t cum_size = 0;   /* number of entries in cum_counts */
    static seq_t *pow10_arr = NULL;
    static size_t pow_size = 0;   /* number of entries in pow10_arr */

    /* Initialise cache */
    if (!cum_counts) {
        cum_counts = (idx_t *)malloc(sizeof(idx_t));
        cum_counts[0] = 0;
        cum_size = 1;
    }

    /* Extend caches until n is within the known range */
    size_t exp = cum_size - 1;  /* current exponent */
    while (cum_counts[exp] <= n) {
        /* Term count for this exponent */
        unsigned tc;
        if (exp <= 1)
            tc = 1;
        else if (exp == 2 || exp == 3)
            tc = 2;
        else if (exp == 4)
            tc = 3;
        else
            tc = 4;

        /* Ensure pow10_arr has value for this exponent */
        while (pow_size <= exp) {
            pow10_arr = (seq_t *)realloc(pow10_arr, (pow_size + 1) * sizeof(seq_t));
            if (pow_size == 0)
                pow10_arr[0] = (seq_t)1;
            else
                pow10_arr[pow_size] = pow10_arr[pow_size - 1] * (seq_t)10;
            pow_size++;
        }

        /* Extend cum_counts */
        idx_t next_cum = cum_counts[exp] + tc;
        cum_counts = (idx_t *)realloc(cum_counts, (exp + 2) * sizeof(idx_t));
        cum_counts[exp + 1] = next_cum;
        cum_size = exp + 2;
        exp++;
    }

    /* Find exponent p such that cum_counts[p] <= n < cum_counts[p+1] */
    size_t p = 0;
    while (p + 1 < cum_size && cum_counts[p + 1] <= n)
        p++;

    /* Offset within the components of 10^p */
    idx_t offset = n - cum_counts[p];

    /* Retrieve 10^p */
    seq_t pow_val = pow10_arr[p];

    /* Break into days, hours, minutes, seconds */
    seq_t d = pow_val / (seq_t)86400;
    seq_t rem1 = pow_val % (seq_t)86400;

    seq_t h = rem1 / (seq_t)3600;
    seq_t rem2 = rem1 % (seq_t)3600;

    seq_t m = rem2 / (seq_t)60;
    seq_t s = rem2 % (seq_t)60;

    /* Collect non‑zero components (hours and minutes may be omitted if zero) */
    seq_t comps[4];
    int cnt = 0;
    if (d > (seq_t)0) comps[cnt++] = d;
    if (h > (seq_t)0) comps[cnt++] = h;
    if (m > (seq_t)0) comps[cnt++] = m;
    comps[cnt++] = s;  /* seconds always present */

    return comps[(int)offset];
}
