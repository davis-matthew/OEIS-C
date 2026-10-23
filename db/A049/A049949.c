/*
 * A049949  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m =
 *          2^(p+1) + 2 - n and p is the unique integer such that 2^p < n - 1
 *          <= 2^(p+1), starting with a(1) = 1, a(2) = 2, and a(3) = 1.
 * https://oeis.org/A049949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=a(1)+a(2)+...+a(n-1)+a(m) for n≥4, where m = 2^(p+1)+2−n
 *            and p is the unique integer such that 2^p < n−1 ≤ 2^(p+1).
 *            Initial values: a(1)=1, a(2)=2, a(3)=1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/pow2.h"

static seq_t *a_arr = NULL;          /* a[1..] */
static seq_t *pref_arr = NULL;       /* prefix sums: pref[i]=a(1)+...+a(i) */
static idx_t cache_cap = 0;          /* allocated capacity (≥max index stored) */
static idx_t cache_size = 0;         /* largest index that has been computed */

/* Ensure capacity for indices up to 'needed' (inclusive).  Grow by factor 2. */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < (size_t)needed)
        new_cap <<= 1;
    seq_t *new_a = (seq_t *)malloc(new_cap * sizeof(seq_t));
    seq_t *new_pref = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_a || !new_pref) {
        /* allocation failed – abort */
        exit(EXIT_FAILURE);
    }
    /* copy existing data */
    for (idx_t i = 1; i <= cache_size; ++i) {
        new_a[i] = a_arr[i];
        new_pref[i] = pref_arr[i];
    }
    /* free old arrays */
    free(a_arr);
    free(pref_arr);
    a_arr = new_a;
    pref_arr = new_pref;
    cache_cap = (idx_t)new_cap;
}

/* Compute the OEIS sequence A049949. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined below offset – return 0 */

    /* compute all values up to n if necessary */
    if (n > cache_size)
        ensure_capacity(n);

    if (cache_size == 0) {
        /* initialise from the first term up to what is needed */
        for (idx_t i = 1; i <= n; ++i) {
            if (i == 1) {
                a_arr[1] = (seq_t)1;
                pref_arr[1] = a_arr[1];
            } else if (i == 2) {
                a_arr[2] = (seq_t)2;
                pref_arr[2] = pref_arr[1] + a_arr[2];
            } else if (i == 3) {
                a_arr[3] = (seq_t)1;
                pref_arr[3] = pref_arr[2] + a_arr[3];
            } else {
                /* recurrence for i >= 4 */
                idx_t p = floor_log2_uint64(i - 2);               /* p satisfies 2^p < i-1 <= 2^(p+1) */
                seq_t two_pow = pow2(p + 1);                       /* 2^(p+1) */
                seq_t m_seq = two_pow + (seq_t)2 - (seq_t)i;         /* compute m as seq_t */
                idx_t m = (idx_t)m_seq;                            /* convert to index (fits in idx_t) */
                seq_t a_m = a_arr[m];
                a_arr[i] = pref_arr[i - 1] + a_m;
                pref_arr[i] = pref_arr[i - 1] + a_arr[i];
            }
        }
        cache_size = n;
    } else if (n > cache_size) {
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1) {
                a_arr[1] = (seq_t)1;
                pref_arr[1] = a_arr[1];
            } else if (i == 2) {
                a_arr[2] = (seq_t)2;
                pref_arr[2] = pref_arr[1] + a_arr[2];
            } else if (i == 3) {
                a_arr[3] = (seq_t)1;
                pref_arr[3] = pref_arr[2] + a_arr[3];
            } else {
                idx_t p = floor_log2_uint64(i - 2);
                seq_t two_pow = pow2(p + 1);
                seq_t m_seq = two_pow + (seq_t)2 - (seq_t)i;
                idx_t m = (idx_t)m_seq;
                seq_t a_m = a_arr[m];
                a_arr[i] = pref_arr[i - 1] + a_m;
                pref_arr[i] = pref_arr[i - 1] + a_arr[i];
            }
        }
        cache_size = n;
    }

    return a_arr[n];
}
