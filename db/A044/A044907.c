/*
 * A044907  Numbers whose base-6 run lengths alternate: odd, even, odd, ...
 * https://oeis.org/A044907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose base‑6 run lengths
 *            alternate odd, even, odd, … (starting with odd)
 * Range:     u8 n <= 40, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* cache for already computed terms */
static seq_t *cache      = NULL;   /* array of terms */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t cache_len  = 0;      /* number of terms stored */
static seq_t last_checked = 0;     /* last integer checked (for scanning) */

/* test whether x satisfies the alternating run‑length rule in base 6 */
static bool valid(seq_t x)
{
    /* handle the trivial case */
    if (x == 0) return false;

    /* find the highest power of 6 <= x */
    seq_t pw = 1;
    while (pw <= x / 6) pw *= 6;

    seq_t prev_digit = (seq_t)-1;   /* sentinel for “no digit yet” */
    seq_t run_len    = 0;
    int    expected  = 1;           /* first run must be odd (1) → toggle to 0 when even */

    /* process digits from most significant to least */
    while (pw > 0) {
        seq_t digit = x / pw;
        x           = x % pw;
        pw          = pw / 6;

        if (prev_digit == (seq_t)-1) {           /* first digit */
            prev_digit = digit;
            run_len    = 1;
        } else if (digit == prev_digit) {       /* same run continues */
            run_len++;
        } else {                                /* new run starts */
            if ((run_len & 1) != expected) {
                return false;                  /* parity mismatch */
            }
            expected ^= 1;                    /* alternate expected parity */
            prev_digit = digit;
            run_len    = 1;
        }
    }

    /* check the last run */
    return ((run_len & 1) == expected);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n == 0, but harmless */

    /* generate terms until we have the n‑th one */
    while (cache_len < (size_t)n) {
        seq_t cand = last_checked + 1;
        last_checked = cand;

        if (valid(cand)) {
            /* grow the cache array if needed */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *new_cache =
                    (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) abort();
                cache     = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }
    return cache[n - 1];
}
