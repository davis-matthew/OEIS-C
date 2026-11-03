/*
 * A032953  Numbers whose base-3 representation Sum_{i=0..m} d(i)*3^(m-i) has
 *          even d(i) for all odd i.
 * https://oeis.org/A032953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 938 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A032953 – numbers whose base‑3 representation has even digits at
   all odd positions (counting from the most significant digit, i.e. d(0)).
   The function implements a brute‑force search with memoized results. */ 
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether a value satisfies the odd‑position‑even rule in base 3. */
static bool valid_b3(seq_t val)
{
    seq_t t = val;
    /* The maximum number of base‑3 digits in a 64‑bit integer is 41.
       An arbitrary‑precision type will use far fewer digits for small
       values that appear in the sequence.  We allocate a buffer that is
       comfortably large for all expected terms in the test suite.  */
    unsigned int digits[64];
    unsigned int len = 0;

    /* Decompose val into base‑3 digits (least significant first). */
    while (t > (seq_t)0) {
        digits[len++] = (unsigned int)(t % (seq_t)3);
        t /= (seq_t)3;
    }
    /* If val == 0 (not used here) we consider it valid. */
    if (len == 0) return true;

    /* Check the rule from the most significant digit. */
    for (unsigned int i = 0; i < len; ++i) {
        unsigned int d = digits[len - 1 - i];   /* i counts from most significant side */
        if ((i & 1) && (d & 1))                /* odd position with odd digit? */
            return false;
    }
    return true;
}

/* Static cache of computed terms. */
static seq_t *cache          = NULL;   /* array holding sequence values */
static idx_t cache_len       = 0;      /* number of values stored */
static idx_t cache_cap       = 0;      /* current capacity of the array */
static seq_t next_candidate  = 1;      /* smallest integer yet unchecked */

/* Return the n-th term (a(1)=1).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of offset – undefined, guard */

    /* Ensure the cache contains at least n values. */
    if (n > cache_len) {
        /* Grow the cache if necessary. */
        if (cache_cap < n) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < n) new_cap *= 2;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();   /* fatal allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Generate additional terms until we have n of them. */
        while (cache_len < n) {
            if (valid_b3(next_candidate))
                cache[cache_len++] = next_candidate;
            ++next_candidate;
        }
    }
    return cache[n - 1];
}
