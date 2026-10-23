/*
 * A043317  a(n)=A033011(n)/14.
 * https://oeis.org/A043317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 444, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the OEIS sequence A043317:
 *  a(n) = the nth positive integer whose base‑169 representation
 *  uses only digits 0…12 and contains no repeated adjacent digits.
 *  The sequence is generated iteratively by building all admissible
 *  numbers in ascending order.
 *
 *  The function returns the nth term with n starting at 1.
 *  A static cache allows repeated calls without re‑generation.
 */

#include <stdlib.h>

/* Ensure capacity is at least 'need' elements.  Used internally only. */
static void ensure_seq_capacity(seq_t **buf, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t newcap = *cap ? *cap << 1 : 16;
        while (newcap < need) newcap <<= 1;
        seq_t *p = (seq_t *)realloc(*buf, newcap * sizeof(seq_t));
        *buf = p;
        *cap = newcap;
    }
}

/* Recursive builder for numbers of fixed length 'depth'.
 * pos   – current position (0 = most significant digit)
 * depth – total number of digits
 * value – value built so far
 * power – 169^(depth-1-pos), i.e. place value of the next digit
 * prev   – previous digit (valid only for pos>0)
 * Cache storage is written into *buf and its length in *len. */
static void gen_recursive(idx_t pos, idx_t depth, seq_t value, seq_t power,
                          int prev, seq_t *buf, size_t *len, size_t *cap)
{
    if (pos == depth) {
        /* Store the completed number. */
        buf[(*len)++] = value;
        return;
    }

    if (pos == 0) {
        /* Leading digit cannot be 0. */
        for (int d = 1; d <= 12; ++d) {
            seq_t new_val = value + (seq_t)d * power;
            gen_recursive(pos + 1, depth, new_val, power / 169, d, buf, len, cap);
        }
    } else {
        for (int d = 0; d <= 12; ++d) {
            if (d == prev) continue;               /* no equal adjacent digits */
            seq_t new_val = value + (seq_t)d * power;
            gen_recursive(pos + 1, depth, new_val, power / 169, d, buf, len, cap);
        }
    }
}

seq_t a(idx_t n)
{
    /*  Static cache that grows as needed.  It holds all generated
     *  admissible numbers in ascending order.  Cached values are
     *  reused for subsequent calls. */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;
    static idx_t  next_len   = 0;               /* length of numbers just generated */

    if (n == 0) return 0;                       /* undefined for n=0, return 0 */

    /* Generate more numbers until we have at least n entries. */
    while (cache_len < n) {
        ++next_len;                              /* increase length to generate */

        /* Count how many numbers will be produced for this length:
         * 12 choices for the first digit (* no leading 0)
         * followed by 12 choices for each remaining digit
         * (0–12 except the previous digit).  Thus 12^next_len. */
        size_t count_new = 1;
        for (int i = 0; i < next_len; ++i) count_new *= 12;

        /* Resize cache to accommodate the new numbers. */
        ensure_seq_capacity(&cache, &cache_cap, cache_len + count_new);

        /* Compute the power of 169 for the most significant digit. */
        seq_t pw = 1;
        for (int i = 1; i < next_len; ++i) pw *= 169;   /* pw = 169^(next_len-1) */

        /* Generate all admissible numbers of this length. */
        gen_recursive(0, next_len, 0, pw, -1, cache, &cache_len, &cache_cap);
    }

    return cache[n - 1];
}
