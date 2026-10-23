/*
 * A049901  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m =
 *          2^(p+1) + 2 - n and p is the unique integer such that 2^p < n - 1
 *          <= 2^(p+1), starting with a(1) = 1, a(2) = 2, and a(3) = 1.
 * https://oeis.org/A049901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{i=1}^{n-1} a(i) - a(m), where m = 2^(p+1)+2-n and p =
 *            floor(log2(n-1)) for n-1 not a power of two, p =
 *            floor(log2(n-1))−1 when n-1 is a power of two. Initial values:
 *            a(1)=1, a(2)=2, a(3)=1
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_log2_uint64.h"

static seq_t *seq  = NULL;          /* stored values a(1) … a(n)   */
static seq_t *pref = NULL;          /* pref[0]=0, pref[k]=∑_{i=1}^k a(i) */
static idx_t seq_cap  = 0;          /* current capacity of seq       */
static idx_t pref_cap = 0;          /* current capacity of pref      */
static idx_t len     = 0;           /* number of terms already computed */

static void ensure_capacity(idx_t needed, seq_t **arr, idx_t *cap)
{
    if (*cap < needed) {
        idx_t new_cap = (*cap > 0) ? (*cap << 1) : needed;
        if (new_cap < needed) new_cap = needed;
        *arr = (seq_t *)realloc(*arr, (size_t)new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* unused, keep defined */

    /* Ensure storage is large enough */
    ensure_capacity(n,          &seq,  &seq_cap);
    ensure_capacity(n + 1,      &pref, &pref_cap);

    /* pref[0] must always be zero */
    pref[0] = (seq_t)0;

    /* Compute terms up to n if necessary */
    if (n > len) {
        for (idx_t i = len + 1; i <= n; ++i) {
            if (i == 1) {
                seq[0] = (seq_t)1;
                pref[1] = pref[0] + seq[0];
            } else if (i == 2) {
                seq[1] = (seq_t)2;
                pref[2] = pref[1] + seq[1];
            } else if (i == 3) {
                seq[2] = (seq_t)1;
                pref[3] = pref[2] + seq[2];
            } else {
                /* Sum of previous values */
                seq_t sum_prev = pref[i - 1];

                /* Compute p according to the definition */
                idx_t x = i - 1;
                idx_t p = floor_log2_uint64(x);
                if (((idx_t)1 << p) == x) {
                    /* when n-1 is a power of two subtract one */
                    p--;
                }

                /* m = 2^(p+1) + 2 - n (1‑based index) */
                idx_t m = ((idx_t)1 << (p + 1)) + 2 - i;

                /* a(m) exists because m >= 1 for all n >= 4 */
                seq_t sub = seq[m - 1];

                seq_t cur = sum_prev - sub;
                seq[i - 1] = cur;
                pref[i] = pref[i - 1] + cur;
            }
        }
        len = n;
    }

    return seq[n - 1];
}
