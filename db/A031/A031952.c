/*
 * A031952  Numbers with exactly two distinct base-7 digits.
 * https://oeis.org/A031952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers with exactly two distinct base-7 digits (A031952)
   This function builds the sequence on the fly by generating all
   base‑7 numbers that use exactly two distinct digits and are sorted.
   For any requested index n≥1 it returns the n-th term. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* comparison function for qsort */
static int cmp_seq(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* Auxiliary function that generates all len‑digit numbers
   with exactly two distinct base‑7 digits and appends them
   (sorted) to the static cache. */
static void generate_len(size_t len)
{
    /* Estimate number of candidates roughly */
    size_t temp_cap = 18 * (1ULL << len);
    seq_t *temp = (seq_t *)malloc(temp_cap * sizeof(seq_t));
    size_t temp_cnt = 0;

    for (int a = 0; a < 7; ++a) {
        for (int b = a + 1; b < 7; ++b) {
            if (a == 0 || b == 0) {
                /* pair contains zero */
                int nonzero = (a == 0) ? b : a;
                size_t total_masks = 1ULL << (len - 1);
                for (size_t mask = 0; mask < total_masks; ++mask) {
                    /* must have at least one zero in the trailing part */
                    if (mask == total_masks - 1) continue;  /* all ones -> all 'nonzero' */
                    seq_t v = nonzero;
                    for (int pos = 1; pos < (int)len; ++pos) {
                        int bit = (mask >> (len - 1 - pos)) & 1;
                        seq_t digit = bit ? nonzero : 0;
                        v = v * 7 + digit;
                    }
                    temp[temp_cnt++] = v;
                }
            } else {
                /* pair without zero */
                size_t total_masks = 1ULL << len;
                for (size_t mask = 0; mask < total_masks; ++mask) {
                    /* at least one of each digit */
                    if (mask == 0 || mask == total_masks - 1) continue;
                    seq_t v = 0;
                    for (int pos = 0; pos < (int)len; ++pos) {
                        int bit = (mask >> (len - 1 - pos)) & 1;
                        seq_t digit = bit ? b : a;
                        v = v * 7 + digit;
                    }
                    temp[temp_cnt++] = v;
                }
            }
        }
    }

    /* sort the new numbers */
    qsort(temp, temp_cnt, sizeof(seq_t), cmp_seq);

    /* static cache variables (initialized on first use) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static size_t next_len_generated = 1;

    /* dynamic reallocation if needed */
    if (cache_len + temp_cnt > cache_cap) {
        size_t new_cap = (cache_len + temp_cnt) * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Out of memory; terminate */
            abort();
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* append sorted block (all numbers of this length are larger than any
       previously generated number, so appending keeps global order) */
    for (size_t i = 0; i < temp_cnt; ++i) {
        cache[cache_len + i] = temp[i];
    }
    cache_len += temp_cnt;

    /* store cache back in static storage for future calls */
    /* Use static variables so that the data persists across calls */
    /* The cache pointer is static, so it's retained automatically. */
    free(temp);

    /* Update the next length pointer for future generation */
    ++next_len_generated;
}

static void ensure_generated(idx_t n)
{
    /* static cache variables across calls */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static size_t next_len_generated = 1; /* generate lengths starting from 2 */

    /* Allocate initial cache storage if needed */
    if (!cache) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_len = 0;
    }

    /* Generate more numbers until we have enough entries */
    while (cache_len < n) {
        /* generate numbers of length next_len_generated */
        size_t temp_cap = 18 * (1ULL << next_len_generated);
        seq_t *temp = (seq_t *)malloc(temp_cap * sizeof(seq_t));
        size_t temp_cnt = 0;

        /* Generate all numbers of the current length (as in generate_len) */
        int len = (int)next_len_generated;
        for (int a = 0; a < 7; ++a) {
            for (int b = a + 1; b < 7; ++b) {
                if (a == 0 || b == 0) {
                    int nonzero = (a == 0) ? b : a;
                    size_t total_masks = 1ULL << (len - 1);
                    for (size_t mask = 0; mask < total_masks; ++mask) {
                        if (mask == total_masks - 1) continue;
                        seq_t v = nonzero;
                        for (int pos = 1; pos < len; ++pos) {
                            int bit = (mask >> (len - 1 - pos)) & 1;
                            seq_t digit = bit ? nonzero : 0;
                            v = v * 7 + digit;
                        }
                        temp[temp_cnt++] = v;
                    }
                } else {
                    size_t total_masks = 1ULL << len;
                    for (size_t mask = 0; mask < total_masks; ++mask) {
                        if (mask == 0 || mask == total_masks - 1) continue;
                        seq_t v = 0;
                        for (int pos = 0; pos < len; ++pos) {
                            int bit = (mask >> (len - 1 - pos)) & 1;
                            seq_t digit = bit ? b : a;
                            v = v * 7 + digit;
                        }
                        temp[temp_cnt++] = v;
                    }
                }
            }
        }
        qsort(temp, temp_cnt, sizeof(seq_t), cmp_seq);

        /* Ensure cache capacity */
        if (cache_len + temp_cnt > cache_cap) {
            size_t new_cap = (cache_len + temp_cnt) * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) abort();
            cache = new_cache;
            cache_cap = new_cap;
        }

        for (size_t i = 0; i < temp_cnt; ++i) {
            cache[cache_len + i] = temp[i];
        }
        cache_len += temp_cnt;
        free(temp);
        ++next_len_generated;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for offset < 1, harmless */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static size_t next_len_generated = 1; /* next length to generate */

    /* Initial allocation */
    if (!cache) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* keep generating until we have enough */
    while (cache_len < n) {
        /* generate numbers of length next_len_generated */
        size_t len = next_len_generated;
        size_t temp_cap = 18 * (1ULL << len);
        seq_t *temp = (seq_t *)malloc(temp_cap * sizeof(seq_t));
        size_t temp_cnt = 0;

        for (int a = 0; a < 7; ++a) {
            for (int b = a + 1; b < 7; ++b) {
                if (a == 0 || b == 0) {
                    int nonzero = (a == 0) ? b : a;
                    size_t total_masks = 1ULL << (len - 1);
                    for (size_t mask = 0; mask < total_masks; ++mask) {
                        if (mask == total_masks - 1) continue;
                        seq_t v = nonzero;
                        for (int pos = 1; pos < (int)len; ++pos) {
                            int bit = (mask >> (len - 1 - pos)) & 1;
                            seq_t digit = bit ? nonzero : 0;
                            v = v * 7 + digit;
                        }
                        temp[temp_cnt++] = v;
                    }
                } else {
                    size_t total_masks = 1ULL << len;
                    for (size_t mask = 0; mask < total_masks; ++mask) {
                        if (mask == 0 || mask == total_masks - 1) continue;
                        seq_t v = 0;
                        for (int pos = 0; pos < len; ++pos) {
                            int bit = (mask >> (len - 1 - pos)) & 1;
                            seq_t digit = bit ? b : a;
                            v = v * 7 + digit;
                        }
                        temp[temp_cnt++] = v;
                    }
                }
            }
        }
        qsort(temp, temp_cnt, sizeof(seq_t), cmp_seq);

        if (cache_len + temp_cnt > cache_cap) {
            size_t new_cap = (cache_len + temp_cnt) * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) abort();
            cache = new_cache;
            cache_cap = new_cap;
        }

        for (size_t i = 0; i < temp_cnt; ++i) {
            cache[cache_len + i] = temp[i];
        }
        cache_len += temp_cnt;
        free(temp);
        ++next_len_generated;
    }

    return cache[n - 1];
}
