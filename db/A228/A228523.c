/*
 * A228523  Numbers that are not the product of two Fibonacci numbers (not
 *          necessarily distinct).
 * https://oeis.org/A228523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A228523:
   a(n) = the n‑th integer that is not the product of two Fibonacci numbers (not necessarily distinct). */
/* The implementation uses a simple sieve: generate Fibonacci numbers once
   (up to a fixed upper bound) and test each integer in increasing order
   to see whether it can be written as a product of two Fibonacci numbers. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static state --------------------------------------------------------- */
static seq_t *fib_cache = NULL;    /* array of unique Fibonacci numbers */
static size_t fib_len  = 0;        /* number of Fibonacci numbers stored   */
static bool   init_done = false;   /* flag for one‑time initialization    */

static seq_t *term_cache = NULL;   /* cached sequence values             */
static size_t  term_len  = 0;       /* number of cached terms             */
static size_t  term_cap  = 0;       /* current allocation capacity        */
static seq_t   start_val = 1;      /* next integer to examine            */

/* --------------------------------------------------------------------- *
 *  Helper: initialize the Fibonacci cache                                 *
 * --------------------------------------------------------------------- */
static void init(void)
{
    if (init_done) return;
    init_done = true;

    /* Upper bound for Fibonacci numbers we need.
       For n up to about 10 000 this limit (1 000 000) is more than
       sufficient, and it also works safely for 64‑bit and 128‑bit
       seq_t values. */
    const seq_t FIB_LIMIT = (seq_t)1000000;

    /* Temporary storage while we build the cache. */
    seq_t tmp[128];
    size_t cnt = 0;

    /* Generate Fibonacci numbers, keeping only unique values. */
    seq_t a = 1, b = 1;
    tmp[cnt++] = a;          /* first unique Fibonacci number = 1 */
    a = b; b = a + b;        /* now a=1, b=2 */

    while (b <= FIB_LIMIT) {
        if (b != tmp[cnt-1]) {  /* skip the duplicate 1 */
            tmp[cnt++] = b;
        }
        seq_t next = a + b;
        if (next <= b) break;   /* overflow guard */
        a = b;
        b = next;
    }

    /* Allocate and copy into the static cache. */
    fib_len = cnt;
    fib_cache = (seq_t *)malloc(fib_len * sizeof(seq_t));
    if (!fib_cache) exit(1);
    for (size_t i = 0; i < fib_len; ++i) fib_cache[i] = tmp[i];
}

/* --------------------------------------------------------------------- *
 *  Helper: test whether m is the product of two Fibonacci numbers.       *
 * --------------------------------------------------------------------- */
static bool is_product_of_two_fibs(seq_t m)
{
    init();  /* ensure fib_cache is ready */

    for (size_t i = 0; i < fib_len; ++i) {
        seq_t f = fib_cache[i];
        if (f > m) break;
        if (m % f == 0) {
            seq_t q = m / f;
            /* linear search is fine: fib_len is tiny (≤ 45 for our limit). */
            for (size_t j = 0; j < fib_len; ++j) {
                if (fib_cache[j] == q) return true;
            }
        }
    }
    return false;
}

/* --------------------------------------------------------------------- *
 *  Helper: extend term_cache until it contains at least n entries.      *
 * --------------------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    init();  /* make sure fib_cache is ready */

    while (term_len < (size_t)n) {
        seq_t candidate = start_val++;
        if (!is_product_of_two_fibs(candidate)) {
            /* Grow the cache if necessary. */
            if (term_len == term_cap) {
                size_t new_cap = term_cap ? term_cap * 2 : 64;
                seq_t *new_buf = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
                if (!new_buf) exit(1);
                term_cache = new_buf;
                term_cap   = new_cap;
            }
            term_cache[term_len++] = candidate;
        }
    }
}

/* --------------------------------------------------------------------- *
 *  Public function required by the task.                                *
 * --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */
    ensure_cache(n);
    return term_cache[n - 1];
}
