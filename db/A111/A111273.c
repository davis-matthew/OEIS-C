/*
 * A111273  a(n) is the smallest divisor of triangular number T(n) := n(n+1)/2
 *          not already in the sequence.
 * https://oeis.org/A111273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest divisor of T(n)=n(n+1)/2 that has not
 *            appeared before
 * Range:     u8 n <= 36, u16 n <= 396, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* static storage for the sequence and a bit set of seen numbers */
static seq_t *seq = NULL;        /* sequence values, 1‑based index  -> seq[i-1] = a(i)   */
static size_t seq_len = 0;       /* number of computed terms */
static size_t seq_cap = 0;       /* allocated capacity for seq                    */

static bool *seen = NULL;        /* seen[n] == true iff value n has appeared   */
static size_t seen_cap = 0;      /* allocated capacity for seen, indices 0 .. seen_cap-1 */

/* helper: ensure seq array can hold at least n elements */
static void ensure_seq_cap(size_t n)
{
    if (seq_cap < n) {
        size_t new_cap = seq_cap? seq_cap * 2 : 4;
        while (new_cap < n) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* helper: ensure seen array can address index d */
static void ensure_seen_cap(uint64_t d)
{
    if (seen_cap <= d) {
        size_t new_cap = seen_cap? seen_cap * 2 : 4;
        while (new_cap <= d) new_cap <<= 1;
        seen = (bool *)realloc(seen, new_cap * sizeof(bool));
        memset(seen + seen_cap, 0, (new_cap - seen_cap) * sizeof(bool));
        seen_cap = new_cap;
    }
}

/* comparator for qsort of uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    if (x < y) return -1;
    return x > y;
}

/* main function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n <= 0, return 0 */

    /* compute up to n if necessary */
    if (n > seq_len) {
        ensure_seq_cap((size_t)n);
        for (size_t i = seq_len; i < (size_t)n; ++i) {
            uint64_t idx = i + 1;                       /* 1‑based n */
            uint64_t T  = idx * (idx + 1) / 2;          /* triangular number */

            /* gather all divisors of T into a temporary array */
            uint64_t divs[4096];
            size_t div_len = 0;
            for (uint64_t d = 1; d * d <= T; ++d) {
                if (T % d == 0) {
                    divs[div_len++] = d;
                    uint64_t d2 = T / d;
                    if (d2 != d) divs[div_len++] = d2;
                }
            }

            /* sort divisors */
            qsort(divs, div_len, sizeof(uint64_t), cmp_uint64);

            /* find the smallest divisor not seen yet */
            uint64_t ans = 0;
            for (size_t j = 0; j < div_len; ++j) {
                uint64_t d = divs[j];
                ensure_seen_cap(d);
                if (!seen[d]) {
                    ans = d;
                    break;
                }
            }
            /* ans must be found (permutation property) */
            seq[i] = (seq_t)ans;
            ensure_seen_cap(ans);
            seen[ans] = true;
        }
        seq_len = (size_t)n;
    } else {
        /* if n is already computed, no action needed */
    }

    return seq[(size_t)n - 1];
}
