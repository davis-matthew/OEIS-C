/*
 * A044982  Numbers whose base-3 representation contains exactly three 0's and
 *          two 1's.
 * https://oeis.org/A044982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number whose base‑3 representation contains exactly three
 *            0's and two 1's (offset 1)
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Cached powers of 3, pow3[i] = 3^i */
static seq_t *pow3      = NULL;
static size_t pow3_len  = 0;
static size_t pow3_cap  = 0;

/* Cached sequence terms */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Current base‑3 length being generated */
static idx_t next_len = 5;  /* smallest length with 5 digits */

static void ensure_pow3(idx_t n)
{
    if (pow3_len > (size_t)n) return;
    size_t new_cap = pow3_cap ? pow3_cap : 4;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(pow3, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    pow3 = tmp;
    pow3_cap = new_cap;
    if (pow3_len == 0) {
        pow3[0] = (seq_t)1;
        pow3_len = 1;
    }
    for (size_t i = pow3_len; i <= (size_t)n; ++i)
        pow3[i] = pow3[i-1] * (seq_t)3;
    pow3_len = n + 1;
}

static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 0 */

    while (cache_len < n) {
        idx_t L = next_len++;            /* current base‑3 length */

        ensure_pow3(L);                  /* need 3^L */
        seq_t base_val = pow3[L] - (seq_t)1;  /* all digits 2 */

        /* Count how many terms of length L exist:
           choose 3 zero positions among L-1 (positions 1..L-1),
           then choose 2 one positions among the remaining L-3 positions. */
        size_t zeroComb = 1;
        for (size_t t = 1; t <= 3; ++t) {
            zeroComb = zeroComb * ((size_t)(L - t));
            zeroComb /= t;
        }
        size_t remCount = (size_t)(L - 3);
        size_t oneComb = 1;
        for (size_t t = 1; t <= 2; ++t) {
            oneComb = oneComb * (remCount - t + 1);
            oneComb /= t;
        }
        size_t termsCap = zeroComb * oneComb;
        seq_t *buf = (seq_t *)malloc(termsCap * sizeof(seq_t));
        if (!buf) exit(1);
        size_t buf_len = 0;

        /* Enumerate all zero position triples. */
        for (idx_t i = 1; i <= L - 3; ++i) {
            for (idx_t j = i + 1; j <= L - 2; ++j) {
                for (idx_t k = j + 1; k <= L - 1; ++k) {
                    /* Gather remaining positions. */
                    idx_t rem[64];
                    size_t rem_len = 0;
                    for (idx_t p = 0; p < L; ++p) {
                        if (p == i || p == j || p == k) continue;
                        rem[rem_len++] = p;
                    }
                    /* Base value minus zero contributions. */
                    seq_t val0 = base_val;
                    val0 -= (seq_t)2 * pow3[L - 1 - i];
                    val0 -= (seq_t)2 * pow3[L - 1 - j];
                    val0 -= (seq_t)2 * pow3[L - 1 - k];

                    /* Choose two one positions. */
                    for (size_t a = 0; a + 1 < rem_len; ++a)
                        for (size_t b = a + 1; b < rem_len; ++b) {
                            seq_t val = val0
                                      - pow3[L - 1 - rem[a]]
                                      - pow3[L - 1 - rem[b]];
                            buf[buf_len++] = val;
                        }
                }
            }
        }

        /* Sort the terms of this length. */
        qsort(buf, buf_len, sizeof(seq_t), cmp_seq_t);

        /* Append sorted terms to the global cache. */
        ensure_cache_capacity(cache_len + buf_len);
        memcpy(cache + cache_len, buf, buf_len * sizeof(seq_t));
        cache_len += buf_len;
        free(buf);
    }

    return cache[n - 1];
}
