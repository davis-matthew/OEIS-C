/*
 * A044891  Base-5 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Base‑5 run lengths alternate even, odd, even,… starting with an even run.
   The code searches forward for the next integer satisfying this property. */

#include <stdbool.h>
#include <stdlib.h>

/* Cached sequence values to allow random‑order queries. */
static seq_t *cache   = NULL;   /* computed terms */
static size_t cache_cap   = 0;   /* allocated capacity */
static size_t cache_len   = 0;   /* number of terms stored */

/* Return true if x in base 5 has run lengths alternating even, odd, even, … starting
   with an even length.  The integer x is at least 6.  The function does not modify
   the argument inside its scope. */
static bool is_valid(seq_t x)
{
    if (x < (seq_t)6) return false;

    /* Highest power of 5 not exceeding x.  Use division to avoid overflow. */
    seq_t pow5 = (seq_t)1;
    while (pow5 <= x / (seq_t)5) pow5 *= (seq_t)5;

    int need_even = 1;      /* first run must be even */
    int prev_digit = -1;    /* digit of current run */
    unsigned run_len = 0;   /* length of current run */

    while (pow5 > (seq_t)0) {
        int d = (int)(x / pow5);
        x %= pow5;
        pow5 /= (seq_t)5;

        if (prev_digit == -1) {          /* start first run */
            prev_digit = d;
            run_len = 1;
        } else if (d == prev_digit) {    /* extend run */
            run_len++;
        } else {                         /* run finished */
            if (need_even) {
                if (run_len & 1U) return false;
            } else {
                if (!(run_len & 1U)) return false;
            }
            need_even = !need_even;
            prev_digit = d;
            run_len = 1;
        }
    }

    /* Check the final run. */
    if (need_even) {
        if (run_len & 1U) return false;
    } else {
        if (!(run_len & 1U)) return false;
    }
    return true;
}

/* Return the nth term (n ≥ 1) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n = 0 */

    /* Expand the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t cand;
        if (cache_len == 0)
            cand = (seq_t)6;
        else
            cand = cache[cache_len - 1] + (seq_t)1;

        /* Find the next valid candidate. */
        while (!is_valid(cand)) cand++;

        /* Ensure sufficient storage capacity. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {   /* allocation failure – clean up and return 0 */
                free(cache);
                cache = NULL;
                cache_cap = 0;
                cache_len = 0;
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = cand;
    }

    return cache[n - 1];
}
