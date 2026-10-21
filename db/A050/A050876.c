/*
 * A050876  Base 3 numbers d(1)...d(j) such that d(i) = d(j+1-i) for all but 2
 *          values of i.
 * https://oeis.org/A050876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th base‑3 number that has exactly one mismatched
 *            symmetric pair of digits (i.e. the base‑3 representation is not
 *            a palindrome but two positions (a pair) are unequal)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation stores previously computed terms in a dynamically
    resized static array.  Each call independently scans numbers from
    the last examined value until n terms are found.  The test for a
    valid number extracts the base‑3 digits into an array and counts
    mismatching symmetric pairs, accepting the number only when the
    count is exactly one.  This algorithm runs in O(n·L) where L is the
    number of base‑3 digits of the n‑th term, easily fast enough for
    n up to ~10 000.

    The code uses only standard C operators and the provided utility
    types.  No external libraries beyond <stdlib.h> are required. */

#include <stdlib.h>

/* local helper: test whether a base‑3 number has exactly one mismatched
   symmetric digit pair. */
static int is_valid(seq_t n) {
    seq_t tmp = n;
    /* store digits least‑significant first; 128 digits suffice for
       all numbers encountered up to the required range. */
    seq_t digits[128];
    size_t len = 0;
    while (tmp > (seq_t)0 && len < sizeof(digits)/sizeof(digits[0])) {
        digits[len++] = tmp % (seq_t)3;
        tmp /= (seq_t)3;
    }

    /* for numbers larger than the array can hold (extremely unlikely
       here) reject them; this keeps the function well defined and
       prevents overrun. */
    if (tmp > (seq_t)0) return 0;

    size_t mismatches = 0;
    for (size_t i = 0; i < len / 2; ++i) {
        if (digits[i] != digits[len - 1 - i]) {
            ++mismatches;
            if (mismatches > 1) return 0;          /* fail early */
        }
    }
    return mismatches == 1;                         /* exactly one pair */
}

/* The required sequence function. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;     /* cache[0] == a(1) */
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static seq_t next_val = (seq_t)3;   /* first candidate to test */

    /* grow cache to hold at least n terms */
    while (cache_size < n) {
        seq_t cand = next_val;
        if (is_valid(cand)) {
            /* store cand */
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t*)realloc(cache,
                                new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failure */
                    /* In production code you might abort or return a
                       sentinel value; here we simply return 0. */
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }
        ++next_val;
    }
    return cache[n - 1];
}
