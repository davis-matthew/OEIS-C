/*
 * A025326  Numbers that are the sum of 3 nonzero squares in exactly 6 ways.
 * https://oeis.org/A025326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 707, u32 n <= 707, u64 n <= 707,
 *            u128 n <= 707, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerates all unordered triples (a,b,c) of positive integers
   with a ≤ b ≤ c and a²+b²+c² up to a limit, counts how many triples give
   each sum, then collects the sums that occur in exactly six different
   ways.  The function grows the sieve limit until it has produced at
   least the nth term.  The search is O(limit^{3/2}), which is fast enough
   to produce n = 10 000 within the time limits. */

#include <stdlib.h>
#include <stdint.h>

/* integer square root (floor) of an unsigned 64‑bit number. */
static idx_t _int_isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * (__int128)mid;
        if (sq == n) return mid;
        if (sq < n) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return lo - 1;
}

/* rebuild the table of sums up to 'limit' and store the numbers with
   exactly six representations in *terms_ptr.  The array is allocated
   fresh, the old one (if any) is freed.  'len_ptr' is set to the
   number of terms produced. */
static void _recompute_terms(idx_t limit, seq_t **terms_ptr, size_t *len_ptr)
{
    /* free old terms array, if present */
    if (*terms_ptr != NULL) {
        free(*terms_ptr);
        *terms_ptr = NULL;
    }
    *len_ptr = 0;

    /* allocate counter array: 0..limit */
    uint8_t *cnt = (uint8_t *)calloc((size_t)limit + 1, sizeof(uint8_t));
    if (!cnt) return;                      /* allocation failure – give up */

    /* pre‑compute squares up to sqrt(limit) */
    idx_t root = _int_isqrt_u64(limit);
    uint64_t *sq = (uint64_t *)malloc((size_t)(root + 1) * sizeof(uint64_t));
    if (!sq) { free(cnt); return; }

    for (idx_t i = 0; i <= root; ++i) sq[i] = (uint64_t)i * (uint64_t)i;

    /* enumerate unordered triples a ≤ b ≤ c */
    for (idx_t a = 1; a <= root; ++a) {
        uint64_t a2 = sq[a];
        for (idx_t b = a; b <= root; ++b) {
            uint64_t sum_ab = a2 + sq[b];
            if (sum_ab > limit) break;
            idx_t max_c = _int_isqrt_u64(limit - sum_ab);
            for (idx_t c = b; c <= max_c; ++c) {
                idx_t n = sum_ab + sq[c];
                uint8_t *p = &cnt[n];
                if (*p < 7) ++(*p);        /* saturate at 7 to keep data small */
            }
        }
    }

    /* collect all n with exactly six representations */
    size_t cap = 128;
    seq_t *arr = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!arr) { free(cnt); free(sq); return; }

    size_t len = 0;
    for (idx_t n = 1; n <= limit; ++n) {
        if (cnt[n] == 6) {
            if (len == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(arr, cap * sizeof(seq_t));
                if (!tmp) break;          /* allocation failure – stop expanding */
                arr = tmp;
            }
            arr[len++] = (seq_t)n;
        }
    }

    free(cnt);
    free(sq);

    *terms_ptr = arr;
    *len_ptr   = len;
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    static int      initialized = 0;
    static seq_t   *terms       = NULL;
    static size_t   terms_len   = 0;
    static idx_t    current_limit = 2000000;      /* initial sieve size */

    /* generate first batch */
    if (!initialized) {
        _recompute_terms(current_limit, &terms, &terms_len);
        initialized = 1;
    }

    /* if the required index is beyond what we have, expand the sieve */
    while (n > terms_len) {
        current_limit <<= 1;                    /* double the limit */
        _recompute_terms(current_limit, &terms, &terms_len);
    }

    if (n == 0 || n > terms_len) return (seq_t)0;    /* undefined for offset < 1 */
    return terms[n - 1];
}
