/*
 * A097155  Numbers that reach the fixed point 89 under iteration of f(x) =
 *          reverse(x) - maxdigit(x).
 * https://oeis.org/A097155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   max_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097155: a(n) = the n‑th integer that eventually reaches the fixed point 89
 * under iteration of f(x) = reverse(x) – maxdigit(x).  The routine searches
 * numbers in ascending order, testing each until the required count is reached. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/max_digit.h"

/* --- helpers ------------------------------------------------------------ */

/* Return the decimal reverse of x. */
static inline seq_t reverse_digits(seq_t x)
{
    seq_t r = 0;
    while (x > 0) {
        seq_t d = x % (seq_t)10;
        r = r * (seq_t)10 + d;
        x /= (seq_t)10;
    }
    return r;
}

/* Test whether the orbit of x under f(x)=reverse(x)-maxdigit(x) reaches 89. */
static inline bool reaches_89(seq_t x)
{
    const uint64_t MAX_STEPS = 5000;
    seq_t cur = x;
    uint64_t steps = 0;

    while (steps < MAX_STEPS && cur != (seq_t)89 && cur != (seq_t)0) {
        seq_t rev = reverse_digits(cur);
        seq_t mx  = max_digit(cur);

        if (rev < mx) return false;          /* would underflow → cannot reach 89 */

        seq_t nxt = rev - mx;
        if (nxt == cur) return false;        /* trapped in a non‑89 cycle */

        cur = nxt;
        ++steps;
    }
    return cur == (seq_t)89;
}

/* --- storage for memoised results -------------------------------------- */

static seq_t *cached_seq = NULL;   /* array of found terms  */
static size_t cached_len  = 0;      /* number of terms stored  */
static size_t cached_cap  = 0;      /* allocated capacity  */
static seq_t next_candidate = 1;    /* next integer to test         */

/* Grows the cache array if necessary. */
static void ensure_cache_space(void)
{
    if (cached_len < cached_cap) return;

    size_t new_cap = cached_cap ? cached_cap * 2 : 64;
    seq_t *new_arr = (seq_t *)realloc(cached_seq, new_cap * sizeof(seq_t));
    if (!new_arr) {
        /* Out of memory – abort, as the sequence cannot be continued. */
        abort();
    }
    cached_seq = new_arr;
    cached_cap = new_cap;
}

/* --- main function ----------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n==0, but harmless */

    while (cached_len < (size_t)n) {

        if (reaches_89(next_candidate)) {
            ensure_cache_space();
            cached_seq[cached_len++] = next_candidate;
        }
        ++next_candidate;
    }

    return cached_seq[(size_t)n - 1];
}
