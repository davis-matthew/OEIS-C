/*
 * A181458  Numbers k such that 41 is the largest prime factor of k^2 - 1.
 * https://oeis.org/A181458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 262, u128 n <= 262,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search all integers k such that the largest prime factor
 * of k^2-1 is 41.  The algorithm enumerates all 41‑smooth numbers up
 * to the maximum k allowed (127855050751), then collects consecutive
 * smooth pairs a, a+2; one of them must be divisible by 41.  The
 * resulting list is sorted and stored once, after which a(n) returns
 * the (n‑1)th element.  The construction is performed only on the
 * first call. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* the primes up to 41 */
static const uint64_t primes_u64[13] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
    17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL, 41ULL
};

/* maximum k that may appear in the sequence (from A175607(13)) */
static const uint64_t MAX_K = 127855050751ULL;
static const uint64_t MAX_SMOOTH = MAX_K + 2ULL;  /* to cover a+2 */

/* dynamic arrays for 41‑smooth numbers and the k‑list */
static seq_t *smooth = NULL;
static size_t smooth_cap = 0, smooth_len = 0;

static seq_t *k_list = NULL;
static size_t k_cap = 0, k_len = 0;

/* local function prototypes */
static void gen_smooth(void);
static void gen_smooth_recursive(size_t idx, uint64_t prod);
static int cmp_seq(const void *p1, const void *p2);
static size_t binary_search(const seq_t *arr, size_t n, seq_t val);
static void build_k_list(void);

/* generate all 41‑smooth numbers up to MAX_SMOOTH */
static void gen_smooth(void)
{
    smooth_cap = 1 << 20;                     /* start with ~1M slots */
    smooth = (seq_t*)malloc(smooth_cap * sizeof(seq_t));
    smooth_len = 0;
    gen_smooth_recursive(0, 1ULL);
    /* sort the vector to allow binary search later */
    qsort(smooth, smooth_len, sizeof(seq_t), cmp_seq);
}

/* recursive enumeration of prime exponents */
static void gen_smooth_recursive(size_t idx, uint64_t prod)
{
    if (idx == 13) {
        if (smooth_len >= smooth_cap) {
            smooth_cap <<= 1;                /* double capacity */
            smooth = (seq_t*)realloc(smooth,
                                     smooth_cap * sizeof(seq_t));
        }
        smooth[smooth_len++] = (seq_t)prod;
        return;
    }

    uint64_t p = primes_u64[idx];
    uint64_t cur = prod;

    while (1) {
        gen_smooth_recursive(idx + 1, cur);
        if (cur > MAX_SMOOTH / p) break;    /* would overflow next step */
        cur *= p;
    }
}

/* comparison function for qsort (ascending) */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t*)p1;
    const seq_t *b = (const seq_t*)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* binary search: first index with arr[i] >= val */
static size_t binary_search(const seq_t *arr, size_t n, seq_t val)
{
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* build the sorted list of k's */
static void build_k_list(void)
{
    k_cap = 1 << 15;                           /* ~32k entries */
    k_list = (seq_t*)malloc(k_cap * sizeof(seq_t));
    k_len = 0;

    for (size_t i = 0; i < smooth_len; ++i) {
        seq_t a = smooth[i];
        seq_t b = a + (seq_t)2;
        if ((uint64_t)b > MAX_SMOOTH) continue;    /* out of enumeration range */

        size_t j = binary_search(smooth, smooth_len, b);
        if (j == smooth_len || smooth[j] != b) continue;

        /* at least one of a or b must be divisible by 41 */
        if (!((a % (seq_t)41ULL == 0) || (b % (seq_t)41ULL == 0)))
            continue;

        seq_t k = a + (seq_t)1;
        if (k_len >= k_cap) {
            k_cap <<= 1;
            k_list = (seq_t*)realloc(k_list,
                                     k_cap * sizeof(seq_t));
        }
        k_list[k_len++] = k;
    }
    /* k_list is already sorted ascending because 'a' increases */
}

/* public entry point as required */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        gen_smooth();
        build_k_list();
        initialized = true;
    }
    if (n == 0 || n > (idx_t)k_len)
        return (seq_t)0;            /* undefined term */
    return k_list[(size_t)(n - 1)];
}
