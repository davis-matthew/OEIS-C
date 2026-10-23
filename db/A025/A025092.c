/*
 * A025092  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (Lucas numbers), t = (F(2), F(3), F(4), ...).
 * https://oeis.org/A025092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{ceil(n/2)} L_i * F_{n-i+3} * where L_i is the
 *            i‑th Lucas number (L1=1, L2=3, …) * and F_j is the j‑th
 *            Fibonacci number (F0=0, F1=1, …)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
#  define SEQ_ALLOC(cap) (new seq_t[(cap)])
#  define SEQ_FREE(ptr)  (delete [] (ptr))
#else
#  define SEQ_ALLOC(cap) ((seq_t*)malloc((cap) * sizeof(seq_t)))
#  define SEQ_FREE(ptr)  free((ptr))
#endif

/* Fibonacci cache: fib_cache[i] = F_i   (F0=0, F1=1) */
static seq_t *fib_cache = NULL;
static size_t fib_len = 0;   /* number of initialized Fibonacci entries */
static size_t fib_cap = 0;   /* allocated capacity */

/* Lucas cache: luc_cache[i] = L_i   (L0=2, L1=1) */
static seq_t *luc_cache = NULL;
static size_t luc_len = 0;   /* number of initialized Lucas entries */
static size_t luc_cap = 0;   /* allocated capacity */

static void ensure_fib(idx_t idx) {
    /* Ensure fib_cache[0..idx] are initialized. */
    if ((idx + 1) <= fib_len) return;

    size_t needed = idx + 1;
    size_t new_cap = fib_cap ? fib_cap : 8;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *new_cache = SEQ_ALLOC(new_cap);
    if (!new_cache) abort();

    /* Copy existing entries. */
    for (size_t i = 0; i < fib_len; ++i) new_cache[i] = fib_cache[i];

    /* Initialize missing entries. */
    if (fib_len == 0) {
        new_cache[0] = (seq_t)0;
        new_cache[1] = (seq_t)1;
        fib_len = 2;
    }
    for (size_t i = fib_len; i < new_cap; ++i)
        new_cache[i] = new_cache[i-1] + new_cache[i-2];

    fib_len = new_cap;
    fib_cap = new_cap;
    SEQ_FREE(fib_cache);
    fib_cache = new_cache;
}

static void ensure_luc(idx_t idx) {
    /* Ensure luc_cache[0..idx] are initialized. */
    if ((idx + 1) <= luc_len) return;

    size_t needed = idx + 1;
    size_t new_cap = luc_cap ? luc_cap : 8;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *new_cache = SEQ_ALLOC(new_cap);
    if (!new_cache) abort();

    for (size_t i = 0; i < luc_len; ++i) new_cache[i] = luc_cache[i];

    if (luc_len == 0) {
        new_cache[0] = (seq_t)2;   /* L0 */
        new_cache[1] = (seq_t)1;   /* L1 */
        luc_len = 2;
    }
    for (size_t i = luc_len; i < new_cap; ++i)
        new_cache[i] = new_cache[i-1] + new_cache[i-2];

    luc_len = new_cap;
    luc_cap = new_cap;
    SEQ_FREE(luc_cache);
    luc_cache = new_cache;
}

seq_t a(idx_t n) {
    /* n is 1‑based, valid for n >= 1. */
    idx_t k = (n + 1) / 2;          /* ceil(n/2) */
    ensure_fib(n + 2);              /* need Fibonacci up to F_{n+2} */
    ensure_luc(k);                  /* need Lucas up to L_{k} */

    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t li = luc_cache[(size_t)i];              /* L_i */
        seq_t fi = fib_cache[(size_t)(n - i + 3)];    /* F_{n-i+3} */
        sum += li * fi;
    }
    return sum;
}
