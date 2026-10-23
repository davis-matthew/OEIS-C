/*
 * A049879  a(n) = |j - k|, where u(n) = u(j) + u(k) is the unique sum of Ulam
 *          numbers described in A002859 (with 1 <= j < k < n).
 * https://oeis.org/A049879
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |j - k|, where u(n) = u(j) + u(k) is the unique sum of
 *            Ulam numbers with initial terms 1, 3. The Ulam sequence is
 *            generated iteratively, storing every term u(i) and the index
 *            difference of its unique representation. Non‑memoized generation
 *            would be O(n^2); this implementation keeps a stati...
 * Range:     u8 n <= 58, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache of Ulam numbers and the |j - k| differences */
static seq_t *ulam = NULL;     /* ulam[0] = 1, ulam[1] = 3 */
static seq_t *diff_ij = NULL;  /* diff_ij[i] = |j - k| for ulam[i] */
static idx_t   ulam_len   = 0;
static idx_t   ulam_cap   = 0;
static idx_t   diff_len   = 0;
static idx_t   diff_cap   = 0;

/* ensure capacity for ulam array */
static void ensure_ulam_cap(idx_t needed)
{
    if (needed <= ulam_cap) return;
    idx_t new_cap = ulam_cap ? ulam_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(ulam, (size_t)new_cap * sizeof(seq_t));
    if (!new_arr) abort();                    /* failure is unrecoverable */
    ulam = new_arr;
    ulam_cap = new_cap;
}

/* ensure capacity for diff array */
static void ensure_diff_cap(idx_t needed)
{
    if (needed <= diff_cap) return;
    idx_t new_cap = diff_cap ? diff_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(diff_ij, (size_t)new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    diff_ij = new_arr;
    diff_cap = new_cap;
}

/* compute the sequence up to at least n terms */
static void compute_upto(idx_t n)
{
    if (ulam_len >= n) return;                /* already computed */

    /* initialise if first call */
    if (ulam_len == 0) {
        ensure_ulam_cap(2);
        ensure_diff_cap(2);
        ulam[0] = (seq_t)1;
        ulam[1] = (seq_t)3;
        diff_ij[0] = 0;   /* unused for first term */
        diff_ij[1] = 0;   /* unused for second term */
        ulam_len = 2;
        diff_len = 2;
    }

    /* extend until we have n terms */
    while (ulam_len < n) {
        /* candidate starts just above current maximum */
        seq_t candidate = ulam[ulam_len - 1] + (seq_t)1;

        while (true) {
            idx_t i = 0;
            idx_t j = ulam_len - 1;
            int count = 0;
            idx_t last_i = 0, last_j = 0;

            while (i < j) {
                seq_t sum = ulam[i] + ulam[j];
                if (sum == candidate) {
                    count++;
                    if (count == 1) {
                        last_i = i;
                        last_j = j;
                    } else break;   /* more than one representation */
                    ++i;
                    --j;
                } else if (sum < candidate) {
                    ++i;
                } else {
                    --j;
                }
            }

            if (count == 1) {
                /* accept candidate */
                ensure_ulam_cap(ulam_len + 1);
                ensure_diff_cap(ulam_len + 1);
                ulam[ulam_len] = candidate;
                diff_ij[ulam_len] = last_j - last_i;   /* zero‑based index difference */
                ++ulam_len;
                ++diff_len;
                break;   /* move to next term */
            } else {
                ++candidate;   /* try next integer */
            }
        }
    }
}

/* public API */
seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;     /* undefined below offset, but return 0 */
    compute_upto(n);
    return diff_ij[n - 1];
}
