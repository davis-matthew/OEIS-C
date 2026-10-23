/*
 * A116695  Digit not appearing in A116670(n).
 * https://oeis.org/A116695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the digit missing from the n‑th minimal 9‑digit number
 *            that uses each decimal digit at most once (missing exactly one
 *            digit)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   numeric_of_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates the numbers in A116670 (the missing‑digit
   numbers) in increasing order by merging ten permutation streams
   (one for each digit that could be missing).  Only the missing digit
   is returned, so the numeric values of the numbers are kept in a
   separate 32‑bit field for comparison and are not returned. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/numeric_of_digits.h"

static inline bool next_permutation(uint8_t *a, size_t n)
{
    int i = (int)n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) --i;
    if (i < 0) return false;
    int j = (int)n - 1;
    while (a[j] <= a[i]) --j;
    uint8_t tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    int l = i + 1, r = (int)n - 1;
    while (l < r) {
        tmp = a[l]; a[l] = a[r]; a[r] = tmp;
        ++l; --r;
    }
    return true;
}

typedef struct {
    uint8_t digits[9];      /* current permutation */
    uint32_t val_num;       /* numeric value for ordering */
    int missing;            /* digit that is missing */
    bool finished;          /* no more permutations for this digit */
} Stream;

/* Initialise a stream for the given missing digit */
static void stream_init(Stream *s, int d)
{
    s->missing = d;
    s->finished = false;

    int pos = 0;
    for (int dig = 0; dig <= 9; ++dig)
        if (dig != d)
            s->digits[pos++] = (uint8_t)dig;

    /* Skip permutations that start with '0' because they are not valid
       9‑digit numbers. */
    if (d != 0) {
        while (s->digits[0] == 0) {
            if (!next_permutation(s->digits, 9)) {
                s->finished = true;
                return;
            }
        }
    }

    s->val_num = numeric_of_digits(s->digits);
}

/* Advance to the next permutation for this stream */
static bool stream_next(Stream *s)
{
    if (s->finished) return false;
    if (!next_permutation(s->digits, 9)) {
        s->finished = true;
        return false;
    }
    s->val_num = numeric_of_digits(s->digits);
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static state */
    static bool init = false;
    static Stream streams[10];
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t generated = 0;   /* number of terms already generated */

    /* Ensure cache capacity */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }
    while (n > cache_cap) {
        cache_cap *= 2;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }

    if (!init) {
        for (int d = 0; d < 10; ++d)
            stream_init(&streams[d], d);
        init = true;
    }

    /* Generate terms up to n */
    while (generated < n) {
        /* Find stream with the smallest current value */
        int best = -1;
        uint32_t best_val = UINT32_MAX;
        for (int d = 0; d < 10; ++d) {
            if (streams[d].finished) continue;
            if (best == -1 || streams[d].val_num < best_val) {
                best = d;
                best_val = streams[d].val_num;
            }
        }

        cache[generated++] = (seq_t)streams[best].missing;
        stream_next(&streams[best]);   /* advance that stream */
    }

    return cache[n - 1];
}
