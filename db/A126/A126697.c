/*
 * A126697  a(0)=1. For n>=1, a(n) = a(n-1) + (largest divisor of n which
 *          occurs among the earlier terms of the sequence).
 * https://oeis.org/A126697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1, a(n)=a(n-1)+L, where L is the largest divisor
 *            of n that occurs among a(0),a(1),...,a(n-1)
 * Range:     u8 n <= 34, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_cache = NULL;  /* array of computed sequence terms */
static idx_t seq_len   = 0;      /* number of terms currently stored (>=1 after init) */
static size_t seq_cap  = 0;      /* capacity of seq_cache */

/* binary search: return 1 if val is present in seq_cache[0..len-1] */
static int seq_contains(const seq_t *arr, idx_t len, seq_t val)
{
    idx_t lo = 0, hi = len - 1;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (arr[mid] < val)
            lo = mid + 1;
        else if (arr[mid] > val)
            hi = mid - 1;
        else
            return 1;
    }
    return 0;
}

/* ensure the cache has at least n+1 terms; compute new terms if needed */
static void ensure_up_to(idx_t n)
{
    if (n < seq_len) return;          /* already computed */

    /* initialise for n==0 */
    if (seq_len == 0) {
        size_t init_cap = (n + 1 > 16) ? n + 1 : 16;
        seq_cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        seq_cap   = init_cap;
        seq_cache[0] = (seq_t)1;
        seq_len = 1;
    }

    /* extend capacity if necessary */
    if ((size_t)(n + 1) > seq_cap) {
        size_t new_cap = seq_cap;
        while (new_cap <= (size_t)(n + 1))
            new_cap = new_cap ? new_cap << 1 : 16;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cap   = new_cap;
    }

    /* compute terms from seq_len up to n */
    for (idx_t i = seq_len; i <= n; ++i) {
        /* gather divisors of i */
        idx_t divs[256];
        idx_t dcnt = 0;
        for (idx_t j = 1; j * j <= i; ++j) {
            if (i % j == 0) {
                divs[dcnt++] = j;
                if (j != i / j) divs[dcnt++] = i / j;
            }
        }
        /* sort divisors descending (insertion sort, small array) */
        for (idx_t a = 1; a < dcnt; ++a) {
            idx_t key = divs[a];
            idx_t b = a;
            while (b > 0 && divs[b - 1] < key) {
                divs[b] = divs[b - 1];
                --b;
            }
            divs[b] = key;
        }

        /* find largest divisor present in earlier terms */
        idx_t largest = 1;  /* 1 is always present */
        for (idx_t k = 0; k < dcnt; ++k) {
            idx_t d = divs[k];
            if (seq_contains(seq_cache, i, (seq_t)d)) {
                largest = d;
                break; /* descending order guarantees this is the largest */
            }
        }
        seq_cache[i] = seq_cache[i - 1] + (seq_t)largest;
    }
    seq_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return seq_cache[n];
}
