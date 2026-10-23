/*
 * A135255  Partial sums of A102659 read as decimal integers.
 * https://oeis.org/A135255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the n-th partial sum of the decimal integers formed by
 *            the n-th Lyndon word over the alphabet {1,2} (lexicographically
 *            sorted). The first few terms are a(1)=1, a(2)=3, a(3)=15,
 *            a(4)=127, …
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* helper: test whether a word consisting of digits 1 or 2 is a Lyndon word.
   It checks primitiveness (no proper period) and lexicographic minimality
   of its rotations via Booth's algorithm. */
static int is_lyndon(const uint8_t *s, int len) {
    /* Primitiveness test */
    for (int d = 1; d < len; ++d) {
        if (len % d != 0) continue;
        bool same = true;
        for (int i = 0; i < len; ++i) {
            if (s[i] != s[i % d]) { same = false; break; }
        }
        if (same) return 0; /* not primitive */
    }

    /* Booth's algorithm to find the smallest rotation */
    int i = 0, j = 1, k = 0;
    while (i < len && j < len && k < len) {
        int a = s[(i + k) % len];
        int b = s[(j + k) % len];
        if (a == b) {
            ++k;
            continue;
        }
        if (a < b) {
            j = j + k + 1;
            k = 0;
        } else {
            i = i + k + 1;
            k = 0;
        }
        if (i == j) ++j;
    }
    int pos = (i < j) ? i : j;
    return (pos == 0);
}

/* cache of partial sums; static state of the generator */
static seq_t *cum_cache = NULL;
static size_t cum_cap   = 0;
static idx_t  cum_cnt   = 0;          /* number of cached terms */
static seq_t  cumulative_sum = 0;     /* current running sum */

/* generator state */
static idx_t  gen_len   = 1;          /* current word length being examined */
static uint64_t gen_idx = 0;          /* index within current length (0 … 2^len-1) */
static uint64_t gen_max = 2;          /* 1ULL << gen_len */

/* ensure cache has capacity for at least 'need' elements, reallocating if necessary */
static void ensure_cache(idx_t need) {
    if (!cum_cache) {
        cum_cap = 64;
        cum_cache = (seq_t *)malloc(cum_cap * sizeof(seq_t));
        if (!cum_cache) exit(1);
    }
    while (cum_cap < (size_t)need) {
        cum_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cum_cache, cum_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cum_cache = tmp;
    }
}

/* main function required by the framework */
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* undefined for offset 1, but harmless */
    /* extend to at least n terms */
    while (cum_cnt < n) {
        /* finish current length if done */
        if (gen_idx == gen_max) {
            ++gen_len;
            gen_idx = 0;
            gen_max = (uint64_t)1 << gen_len;
        }

        /* Build the candidate word of length gen_len */
        uint8_t word[128]; /* 128 > maximum expected length for safety */
        for (int pos = 0; pos < (int)gen_len; ++pos) {
            uint64_t bit = (gen_idx >> (gen_len - pos - 1)) & 1ULL;
            word[pos] = (uint8_t)(bit ? 2 : 1);
        }

        if (is_lyndon(word, (int)gen_len)) {
            /* convert to decimal integer */
            seq_t val = 0;
            for (int i = 0; i < (int)gen_len; ++i)
                val = val * (seq_t)10 + (seq_t)word[i];

            cumulative_sum += val;
            ensure_cache(cum_cnt + 1);
            cum_cache[cum_cnt++] = cumulative_sum;
        }

        ++gen_idx;
    }

    return cum_cache[n - 1];
}
