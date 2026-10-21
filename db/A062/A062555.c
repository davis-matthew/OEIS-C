/*
 * A062555  Squares that are the concatenation of three numbers, one of which
 *          is the sum of the other two.
 * https://oeis.org/A062555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search method: iterate through squares and test whether the
   decimal representation can be split into three non‑zero parts
   a, b, c with a + b = c or a + c = b or b + c = a. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: number of decimal digits of a positive seq_t.
   The loop terminates when x becomes zero; zero itself never occurs
   in this code because we only call this for positive squares. */
static size_t digit_len(seq_t x)
{
    size_t len = 0;
    while (x > 0) {
        ++len;
        x /= (seq_t)10;
    }
    return len;
}

/* Dynamically grow a table of powers of ten:
      pow10[k] = 10^k  (k ≥ 0).
   The table is lazily extended as needed. */
static seq_t *pow10 = NULL;
static size_t pow10_cap = 0;

static void ensure_pow10(size_t maxk)
{
    if (pow10_cap > maxk) return;
    size_t new_cap = pow10_cap ? pow10_cap * 2 : 1;
    while (new_cap <= maxk) new_cap <<= 1;
    seq_t *new_pow = (seq_t *)realloc(pow10, new_cap * sizeof(seq_t));
    if (!new_pow) exit(1);              /* allocation failed */
    pow10 = new_pow;
    if (pow10_cap == 0) {
        pow10[0] = (seq_t)1;
        pow10_cap = 1;
    }
    for (size_t i = pow10_cap; i < new_cap; ++i)
        pow10[i] = pow10[i - 1] * (seq_t)10;
    pow10_cap = new_cap;
}

/* Test whether s is a square that satisfies the condition. */
static bool is_valid_square(seq_t s)
{
    size_t len = digit_len(s);
    if (len < 3) return false;                   /* need at least 3 parts */
    ensure_pow10(len);                            /* need powers up to len */

    for (size_t l3 = 1; l3 <= len - 2; ++l3) {    /* length of last part (c) */
        seq_t pow3 = pow10[l3];
        seq_t c    = s % pow3;
        seq_t tmp  = s / pow3;                   /* remaining prefix a|b */

        for (size_t l2 = 1; l2 <= len - l3 - 1; ++l2) { /* length of middle part (b) */
            seq_t pow2 = pow10[l2];
            seq_t b    = tmp % pow2;
            seq_t a    = tmp / pow2;
            size_t l1 = len - l3 - l2;            /* length of first part (a) */

            /* No leading zeros in any part: part must be >= 10^(len-1)  */
            if (a < pow10[l1 - 1] || b < pow10[l2 - 1] || c < pow10[l3 - 1])
                continue;

            if (a + b == c || a + c == b || b + c == a)
                return true;
        }
    }
    return false;
}

/* Cache for computed terms (1‑indexed). */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static idx_t   next_i    = 1;   /* next integer whose square hasn’t been checked */

/* Append a new term to the cache, expanding the array if necessary. */
static void append_term(seq_t v)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = v;
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 0, guard */

    /* Generate terms until we have at least n terms cached. */
    while (cache_len < (size_t)n) {
        seq_t i_seq = (seq_t) next_i;
        seq_t square = i_seq * i_seq;   /* for seq_t = uint64_t and smaller values this is safe;
                                           for larger types the multiplication uses the
                                           full capacity of seq_t. */
        if (is_valid_square(square))
            append_term(square);
        ++next_i;
    }
    return cache[n - 1];
}
