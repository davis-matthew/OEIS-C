/*
 * A044923  Base 7 run lengths form a palindrome.
 * https://oeis.org/A044923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Base‑7 run lengths form a palindrome (OEIS A044923)
   a(n) = the n‑th positive integer whose representation in base 7
   has run lengths that read the same forwards and backwards.
   For example 24 (base‑7: 34) has run lengths [1,1], a palindrome.
   The function computes values on demand and caches them for future
   calls. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff the run lengths of the base‑7 representation of x
   form a palindrome. */
static bool is_pal_runlen(seq_t x)
{
    if (x == 0) return true;          /* not used, but harmless */

    /* The number of runs is at most the number of digits; 256 is
       ample for the range needed (10 000th term fits within <2^256). */
    unsigned runs[256];
    size_t rlen = 0;
    int curr_len = 0;
    unsigned curr_digit = 0;

    seq_t temp = x;
    while (temp > 0) {
        unsigned digit = (unsigned)(temp % 7);   /* digit in base 7 */
        if (curr_len == 0) {                    /* first digit */
            curr_digit = digit;
            curr_len = 1;
        } else if (digit == curr_digit) {       /* same run */
            curr_len++;
        } else {                                /* new run */
            runs[rlen++] = (unsigned)curr_len;
            curr_digit = digit;
            curr_len   = 1;
        }
        temp /= 7;
    }
    runs[rlen++] = (unsigned)curr_len;          /* final run */

    for (size_t i = 0; i < rlen / 2; ++i) {
        if (runs[i] != runs[rlen - 1 - i])
            return false;
    }
    return true;
}

/* External linkage: compute the n‑th term of the sequence.
   (n >= 1; behaviour for n == 0 is undefined but return 0.) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static seq_t *cache = NULL;   /* cached terms */
    static size_t cap   = 0;      /* capacity of cache */
    static size_t len   = 0;      /* number of cached terms */
    static seq_t next_candidate = 1;  /* smallest integer to test next */

    size_t target = (size_t)n;
    while (len < target) {
        /* Grow the cache if necessary */
        if (cap == len) {
            size_t new_cap = cap ? cap * 2 : 16;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();          /* allocation failed */
            cache = tmp;
            cap   = new_cap;
        }

        /* Find the next integer whose base‑7 run lengths form a palindrome */
        while (!is_pal_runlen(next_candidate))
            next_candidate++;

        cache[len++] = next_candidate;
        next_candidate++;
    }
    return cache[target - 1];
}
