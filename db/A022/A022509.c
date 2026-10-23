/*
 * A022509  Describe previous term from the right (method A - initial term is
 *          5).
 * https://oeis.org/A022509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 5
   For n >= 1, a(n) = describe_term_from_right(a(n‑1))
   where describe_term_from_right reads the decimal digits of its
   argument from least‑significant to most‑significant, groups
   consecutive identical digits, and for each group appends the
   decimal count followed by the digit itself (in this order).
   E.g. 1511 → “two 1's, one 5, one 1” → 211511. */

#include <stdlib.h>
#include <stddef.h>

/* ---------- Cache for memoisation ---------- */
static seq_t *cache = NULL;          /* array of computed terms          */
static size_t cache_size = 0;        /* number of terms stored           */
static size_t cache_cap  = 0;        /* current allocation capacity      */

/* Ensure the cache can hold at least 'min' elements. */
static void ensure_cache_capacity(size_t min)
{
    if (min <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap < min) new_cap = min;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------- Generate the next term of the sequence ---------- */
static seq_t next_value(seq_t cur)
{
    seq_t result = 0;
    if (cur == 0) return 0;   /* not expected for this sequence */

    int prev_digit = -1;      /* last processed digit (0-9) */
    seq_t count = 0;          /* current group size          */

    /* Process digits from least‑significant to most‑significant. */
    while (cur > 0) {
        unsigned d = (unsigned)(cur % 10);
        cur /= 10;

        if (prev_digit == -1) {          /* first digit encountered */
            prev_digit = d;
            count = 1;
        } else if (d == prev_digit) {    /* same group continues   */
            count++;
        } else {                         /* new group starts       */
            /* Flush previous group: append its count and digit. */
            char dig_buf[128];
            size_t dig_len = 0;
            seq_t tmp_cnt = count;
            do {                          /* produce count digits in reverse order */
                dig_buf[dig_len++] = '0' + (unsigned)(tmp_cnt % 10);
                tmp_cnt /= 10;
            } while (tmp_cnt > 0);

            /* Append count digits from most to least significant. */
            for (size_t k = 0; k < dig_len; ++k)
                result = result * (seq_t)10 + (seq_t)(dig_buf[dig_len - 1 - k] - '0');

            /* Append the digit itself. */
            result = result * (seq_t)10 + (seq_t)prev_digit;

            /* Start new group. */
            prev_digit = d;
            count = 1;
        }
    }

    /* Flush the last group (if any). */
    if (prev_digit != -1) {
        char dig_buf[128];
        size_t dig_len = 0;
        seq_t tmp_cnt = count;
        do {
            dig_buf[dig_len++] = '0' + (unsigned)(tmp_cnt % 10);
            tmp_cnt /= 10;
        } while (tmp_cnt > 0);

        for (size_t k = 0; k < dig_len; ++k)
            result = result * (seq_t)10 + (seq_t)(dig_buf[dig_len - 1 - k] - '0');

        result = result * (seq_t)10 + (seq_t)prev_digit;
    }

    return result;
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    /* Initialise cache if needed. */
    if (cache_size == 0) {
        ensure_cache_capacity(16);
        cache[0] = (seq_t)5;
        cache_size = 1;
    }

    /* Return cached value if already computed. */
    if (n < cache_size) return cache[n];

    /* Grow cache to accommodate 'n'. */
    ensure_cache_capacity((size_t)n + 1);

    /* Compute missing terms sequentially. */
    for (size_t i = cache_size; i <= (size_t)n; ++i)
        cache[i] = next_value(cache[i - 1]);

    cache_size = (size_t)n + 1;
    return cache[n];
}
