/*
 * A022809  a(n) = F(n+3) + c(n) where F(k) is k-th Fibonacci number and c(n)
 *          is n-th number that is 1 or 2 or is not a Fibonacci number.
 * https://oeis.org/A022809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F(n+3) + c(n) where c(n) is the n-th positive integer
 *            that is 1 or 2 or not a Fibonacci number
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>


static seq_t *fib = NULL;
static idx_t fib_len = 0;
static idx_t fib_cap = 0;

static seq_t *c_cache = NULL;
static idx_t c_len = 0;
static idx_t c_cap = 0;
static seq_t next_val = 1;

/* Ensure fib array is initialized */
static void fib_init(void) {
    if (!fib) {
        fib_cap = 2;
        fib = (seq_t*)malloc(fib_cap * sizeof(seq_t));
        fib[0] = (seq_t)0;
        fib[1] = (seq_t)1;
        fib_len = 2;
    }
}

/* Extend fib so that fib[k] is available (0‑based indexing) */
static void ensure_fib_index(idx_t k) {
    fib_init();
    while (fib_len <= k) {
        if (fib_len >= fib_cap) {
            fib_cap = fib_cap * 2;
            seq_t *tmp = (seq_t*)realloc(fib, fib_cap * sizeof(seq_t));
            if (!tmp) abort();
            fib = tmp;
        }
        fib[fib_len] = fib[fib_len-1] + fib[fib_len-2];
        ++fib_len;
    }
}

/* Extend fib so that the last element is >= v */
static void ensure_fib_up_to_value(seq_t v) {
    fib_init();
    while (fib_len >= 2 && fib[fib_len-1] < v) {
        if (fib_len >= fib_cap) {
            fib_cap = fib_cap * 2;
            seq_t *tmp = (seq_t*)realloc(fib, fib_cap * sizeof(seq_t));
            if (!tmp) abort();
            fib = tmp;
        }
        fib[fib_len] = fib[fib_len-1] + fib[fib_len-2];
        ++fib_len;
    }
}

/* Binary search for Fibonacci membership */
static int member_fib(seq_t x) {
    if (!fib) return 0;
    idx_t l = 0, r = fib_len;
    while (l < r) {
        idx_t m = (l + r) >> 1;
        if (fib[m] == x) return 1;
        else if (fib[m] < x) l = m + 1;
        else r = m;
    }
    return 0;
}

/* Build c(n) cache up to n terms */
static void ensure_c_len(idx_t n) {
    if (!c_cache) {
        c_cap = 64;
        c_cache = (seq_t*)malloc(c_cap * sizeof(seq_t));
        c_len = 0;
        next_val = 1;
    }
    while (c_len < n) {
        seq_t cand = next_val;
        ensure_fib_up_to_value(cand);
        if (cand == (seq_t)1 || cand == (seq_t)2 || !member_fib(cand)) {
            if (c_len >= c_cap) {
                c_cap = c_cap * 2;
                seq_t *tmp = (seq_t*)realloc(c_cache, c_cap * sizeof(seq_t));
                if (!tmp) abort();
                c_cache = tmp;
            }
            c_cache[c_len] = cand;
            ++c_len;
        }
        ++next_val;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for offset 1, but safe */
    ensure_c_len(n);
    ensure_fib_index(n + 3);
    return fib[n + 3] + c_cache[n - 1];
}
