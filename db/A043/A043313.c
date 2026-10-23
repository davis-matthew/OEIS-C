/*
 * A043313  a(n)=A033007(n)/10.
 * https://oeis.org/A043313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043313: numbers whose base‑81 representation uses only the
 * digits 0–8 and has no two equal adjacent digits.  The function
 * generates all such numbers in increasing order and returns the
 * n‑th term (n ≥ 1). */

#include <stdlib.h>

/* Append a value to a dynamically growing seq_t array. */
static void append_to_cache(seq_t val, seq_t **cache, size_t *size, size_t *cap)
{
    if (*size == *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        *cache = tmp;
        *cap = new_cap;
    }
    (*cache)[(*size)++] = val;
}

/* Recursive generator: build a base‑81 number of a given length. */
static void generate_rec(idx_t pos, int prev_digit, seq_t acc,
                         idx_t len, const seq_t *pow81,
                         seq_t **cache, size_t *size, size_t *cap)
{
    if (pos == len) {                      /* full length reached */
        append_to_cache(acc, cache, size, cap);
        return;
    }

    int start = (pos == 0) ? 1 : 0;        /* leading digit cannot be 0 */
    int exp   = (int)(len - 1 - pos);      /* power of 81 for this digit */
    seq_t mult = pow81[exp];

    for (int d = start; d <= 8; ++d) {
        if (pos > 0 && d == prev_digit) continue;
        generate_rec(pos + 1, d, acc + (seq_t)d * mult,
                     len, pow81, cache, size, cap);
    }
}

/* Generate all numbers of a given length and store them. */
static void generate_of_length(idx_t len, const seq_t *pow81,
                               seq_t **cache, size_t *size, size_t *cap)
{
    generate_rec(0, -1, (seq_t)0, len, pow81,
                 cache, size, cap);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Persistent static state across calls. */
    static seq_t *cache   = NULL;         /* sorted array of terms */
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    static idx_t current_len = 0;          /* largest length processed */
    static seq_t *pow81 = NULL;            /* powers of 81 */
    static size_t pow_cap = 0;

    static idx_t generated = 0;            /* terms generated so far */

    /* Keep generating until we have at least n terms. */
    while (generated < n) {
        current_len++;                     /* next length */

        /* Ensure pow81 contains 81^0 … 81^(current_len-1). */
        if (pow_cap < (size_t)current_len) {
            seq_t *tmp = (seq_t *)realloc(pow81,
                    current_len * sizeof(seq_t));
            if (tmp == NULL) abort();
            pow81 = tmp;
            pow81[0] = (seq_t)1;
            for (idx_t i = 1; i < current_len; ++i)
                pow81[i] = pow81[i - 1] * (seq_t)81;
            pow_cap = current_len;
        }

        /* Generate all valid numbers of the new length. */
        generate_of_length(current_len, pow81,
                           &cache, &cache_len, &cache_cap);

        /* Count how many numbers of this length were added. */
        idx_t count = 1;
        for (idx_t i = 0; i < current_len; ++i)
            count *= (idx_t)8;             /* 8 choices each position */
        generated += count;
    }

    /* Return the requested term (1‑based). */
    return cache[n - 1];
}
