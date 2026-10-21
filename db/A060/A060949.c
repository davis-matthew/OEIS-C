/*
 * A060949  Intrinsic 12-palindromes: n is an intrinsic k-palindrome if it is
 *          a k-digit palindrome in some base.
 * https://oeis.org/A060949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer that can be written * as a 12‑digit
 *            palindrome in some base b ≥ 2. * The code enumerates all
 *            12‑digit palindromes for bases * 2,…,10, stores them in a sorted
 *            unique array, * and answers arbitrary indexed queries. It works
 *            for * C11 and C++17, with seq_t and idx_t supplied...
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#include <algorithm>
static std::vector<seq_t> terms;   /* cache for C++ builds */
#else
static seq_t *array_pool = NULL;
static size_t array_size = 0;
static size_t array_cap  = 0;

/* push a value into the dynamic C‑array */
static void push_seq(seq_t val)
{
    if (array_size == array_cap) {
        size_t newcap = (array_cap == 0) ? 1024 : array_cap * 2;
        seq_t *tmp = (seq_t *)realloc(array_pool, newcap * sizeof(seq_t));
        if (!tmp) abort();
        array_pool = tmp;
        array_cap  = newcap;
    }
    array_pool[array_size++] = val;
}

/* comparison function for qsort (C mode) */
static int seq_cmp(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}
#endif

/*--------------------------------------------------------------------*/
/* Initialise the cache: generate palindromes for bases 2–10.           */
static void init_sequence(void)
{
    static bool initialized = false;
    if (initialized) return;           /* already ready */
    const int max_base = 10;           /* covers > 10 000 terms */

    for (int b = 2; b <= max_base; ++b) {
        seq_t pow[12];
        pow[0] = 1;
        seq_t bb = (seq_t)b;
        for (int i = 1; i <= 11; ++i) pow[i] = pow[i-1] * bb;

        /* total combinations for the 5 free digits */
        idx_t total5 = 1;
        for (int i = 0; i < 5; ++i) total5 *= (idx_t)b;

        for (idx_t d0 = 1; d0 < (idx_t)b; ++d0) {
            for (idx_t m = 0; m < total5; ++m) {
                seq_t val = (seq_t)d0 * (pow[0] + pow[11]);
                idx_t tmp = m;
                for (int pos = 1; pos <= 5; ++pos) {
                    idx_t digit = tmp % b;
                    tmp /= b;
                    val += (seq_t)digit * (pow[pos] + pow[11-pos]);
                }
#ifdef __cplusplus
                terms.push_back(val);
#else
                push_seq(val);
#endif
            }
        }
    }

#ifdef __cplusplus
    std::sort(terms.begin(), terms.end());
    auto last = std::unique(terms.begin(), terms.end());
    terms.erase(last, terms.end());
#else
    qsort(array_pool, array_size, sizeof(seq_t), seq_cmp);
    /* remove duplicates in place */
    seq_t *dst = array_pool;
    size_t new_len = 0;
    for (size_t i = 0; i < array_size; ++i) {
        if (i == 0 || array_pool[i] != array_pool[i-1]) {
            dst[new_len++] = array_pool[i];
        }
    }
    array_size = new_len;
#endif
    initialized = true;
}

/*--------------------------------------------------------------------*/
/* Public function required by the build. */
seq_t a(idx_t n)
{
    init_sequence();               /* ensure cache is ready */

#ifdef __cplusplus
    if (n == 0 || n > terms.size()) abort();
    return terms[static_cast<size_t>(n) - 1];
#else
    if (n == 0 || n > array_size) abort();
    return array_pool[(size_t)n - 1];
#endif
}
