/*
 * A154391  Terms of A123466 which have a one-to-one correspondence between
 *          every run of 1's and 0's of the same length.
 * https://oeis.org/A154391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1907, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the nth smallest integer such that the multiset of
   run lengths of 1's equals that of 0's in its binary representation. */
#include <stdlib.h>
#include <stdbool.h>

static int cmp_int(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

/* Test whether x satisfies the run‑length correspondence property */
static bool has_property(seq_t x)
{
    if (x == 0) return false;                 /* binary representation not empty */

    /* collect run lengths of 1's and 0's into dynamic arrays */
    int *ones = NULL, *zeros = NULL;
    int one_cap = 0, zero_cap = 0;
    int one_cnt = 0, zero_cnt = 0;

    /* determine bit length */
    idx_t bits = 0;
    seq_t tmp = x;
    while (tmp) {
        tmp >>= 1;
        ++bits;
    }

    int last_bit = -1;   /* -1 = undefined, 0 or 1 valid */
    int run_len = 0;

    for (idx_t i = bits; i > 0; --i) {
        int bit = (int)((x >> (i - 1)) & 1);
        if (bit == last_bit) {
            ++run_len;
        } else {
            if (last_bit != -1) {
                if (last_bit == 1) {
                    if (one_cnt == one_cap) {
                        one_cap = (one_cap == 0) ? 16 : one_cap * 2;
                        ones = (int *)realloc(ones, one_cap * sizeof(int));
                    }
                    ones[one_cnt++] = run_len;
                } else {
                    if (zero_cnt == zero_cap) {
                        zero_cap = (zero_cap == 0) ? 16 : zero_cap * 2;
                        zeros = (int *)realloc(zeros, zero_cap * sizeof(int));
                    }
                    zeros[zero_cnt++] = run_len;
                }
            }
            last_bit = bit;
            run_len = 1;
        }
    }

    /* store the last run */
    if (last_bit == 1) {
        if (one_cnt == one_cap) {
            one_cap = (one_cap == 0) ? 16 : one_cap * 2;
            ones = (int *)realloc(ones, one_cap * sizeof(int));
        }
        ones[one_cnt++] = run_len;
    } else if (last_bit == 0) {
        if (zero_cnt == zero_cap) {
            zero_cap = (zero_cap == 0) ? 16 : zero_cap * 2;
            zeros = (int *)realloc(zeros, zero_cap * sizeof(int));
        }
        zeros[zero_cnt++] = run_len;
    }

    /* number of runs of 1's and 0's must match */
    if (one_cnt != zero_cnt) {
        free(ones);
        free(zeros);
        return false;
    }

    /* sort the run lengths and compare */
    qsort(ones,  one_cnt, sizeof(int), cmp_int);
    qsort(zeros, zero_cnt, sizeof(int), cmp_int);

    bool ok = true;
    for (int i = 0; i < one_cnt; ++i) {
        if (ones[i] != zeros[i]) { ok = false; break; }
    }

    free(ones);
    free(zeros);
    return ok;
}

/* ------------------------------------------------------------------ */
/* The following part is compiled by a C compiler                           */
/*-------------------------------------------------------------------*/
#ifndef __cplusplus

static seq_t *cache          = NULL;
static idx_t cached_terms    = 0;
static seq_t last_checked    = 1;   /* last integer examined */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n==0, safely return 0 */

    /* return cached term if already computed */
    if (n <= cached_terms)
        return cache[n - 1];

    /* extend cache until the nth term is found */
    while (cached_terms < n) {
        ++last_checked;

        if (has_property(last_checked)) {
            cache = (seq_t *)realloc(cache, (cached_terms + 1) * sizeof(seq_t));
            cache[cached_terms++] = last_checked;
        }
    }

    return cache[n - 1];
}

#else  /* C++ implementation ----------------------------------------------------- */
#include <vector>

static std::vector<seq_t> cache;
static seq_t last_checked = 1;   /* last integer examined */

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    while (cache.size() < n) {
        ++last_checked;
        if (has_property(last_checked))
            cache.push_back(last_checked);
    }

    return cache[n - 1];
}
#endif
