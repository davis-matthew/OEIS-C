/*
 * A100388  a(n) = Bell(n) + Fibonacci(n).
 * https://oeis.org/A100388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Bell(n) + Fibonacci(n)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* static memoisation storage */
#ifdef __cplusplus
static std::vector<seq_t> bell;
static std::vector<seq_t> fib;
static idx_t bell_len = 0;
static idx_t fib_len  = 0;
#else
static seq_t *bell = NULL;
static size_t bell_cap = 0;
static idx_t bell_len = 0;
static seq_t *fib = NULL;
static size_t fib_cap = 0;
static idx_t fib_len  = 0;
#endif

#if !defined(__cplusplus)
static void grow_array(seq_t **arr, size_t *cap, size_t new_cap)
{
    seq_t *tmp = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);           /* out of memory */
    *arr = tmp;
    *cap = new_cap;
}
#endif

static void ensure_bell(idx_t n)
{
#ifdef __cplusplus
    if (bell_len > n) return;
    if (bell_len == 0) {
        bell.reserve(n + 1);
        bell.push_back((seq_t)1);   /* B0 = 1 */
        ++bell_len;
    }
    while (bell_len <= n) {
        const idx_t m = bell_len;
        seq_t sum = 0;
        seq_t binom = 1;            /* C(m-1,0) */
        for (idx_t k = 0; k <= m-1; ++k) {
            sum += binom * bell[k];
            binom = binom * (seq_t)(m - 1 - k) / (seq_t)(k + 1);
        }
        bell.push_back(sum);
        ++bell_len;
    }
#else
    if (bell_len > n) return;
    if (bell_len == 0) {
        grow_array(&bell, &bell_cap, 32);
        bell[0] = (seq_t)1;          /* B0 = 1 */
        ++bell_len;
    }
    while (bell_len <= n) {
        const idx_t m = bell_len;
        seq_t sum = 0;
        seq_t binom = 1;
        for (idx_t k = 0; k <= m-1; ++k) {
            sum += binom * bell[k];
            binom = binom * (seq_t)(m - 1 - k) / (seq_t)(k + 1);
        }
        if (bell_len == bell_cap) grow_array(&bell, &bell_cap, bell_cap*2+1);
        bell[bell_len] = sum;
        ++bell_len;
    }
#endif
}

static void ensure_fib(idx_t n)
{
#ifdef __cplusplus
    if (fib_len > n) return;
    if (fib_len == 0) {
        fib.reserve(n + 1);
        fib.push_back((seq_t)0);     /* F0 = 0 */
        fib.push_back((seq_t)1);     /* F1 = 1 */
        fib_len = 2;
    }
    while (fib_len <= n) {
        fib.push_back(fib[fib_len - 1] + fib[fib_len - 2]);
        ++fib_len;
    }
#else
    if (fib_len > n) return;
    if (fib_len == 0) {
        grow_array(&fib, &fib_cap, 32);
        fib[0] = (seq_t)0;
        fib[1] = (seq_t)1;
        fib_len = 2;
    }
    while (fib_len <= n) {
        if (fib_len == fib_cap) grow_array(&fib, &fib_cap, fib_cap*2+1);
        fib[fib_len] = fib[fib_len - 1] + fib[fib_len - 2];
        ++fib_len;
    }
#endif
}

seq_t a(idx_t n)
{
    ensure_bell(n);
    ensure_fib(n);
    return bell[n] + fib[n];
}
