/*
 * A023502  a(n) = b(n) + d(n), where b(n) = (n-th Fibonacci number > 2 ) and
 *          d(n) = (n-th number that is 1, 2, or 3, or is not a Lucas number).
 * https://oeis.org/A023502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n) + d(n) b(n) is the (n‑th) Fibonacci number > 2 ->
 *            Fibonacci(n+4) d(n) is the n‑th integer that is 1, 2, or 3, or
 *            is NOT a Lucas number > 3 Offset: a(0) is the first term listed.
 *            The function is memoised and works for any order of calls
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   fib, init_lucas_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/fib.h"
#include "../utils/init_lucas_array.h"

/* Grow a dynamically allocated array of seq_t */
static void grow_seq_t_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* ---- b(n) : Fibonacci(n+4) ---- */
    seq_t b = fib((idx_t)(n + 4));

    /* ---- d(n) : find nth number that is 1,2,3, or not a Lucas number >3 ---- */
    static seq_t *d_cache = NULL;          /* array of cached d values */
    static size_t d_len   = 0;             /* number of filled entries */
    static size_t d_cap   = 0;             /* current capacity */

    /* static state for generating d values */
    static uint64_t lucas_ptr  = 0;        /* index of next Lucas number to skip */
    static uint64_t cur        = 1;        /* current integer candidate */
    static uint64_t *lucas     = NULL;     /* array of Lucas numbers >3 */
    static size_t  lucas_cnt   = 0;
    static bool    lucas_inited= false;

    if (!lucas_inited) {
        init_lucas_array(&lucas, &lucas_cnt);
        lucas_inited = true;
    }

    /* Ensure cache contains d(n) */
    while (d_len <= (size_t)n) {
        /* Skip Lucas numbers > 3 */
        while (lucas_ptr < lucas_cnt && cur == lucas[lucas_ptr]) {
            ++cur;
            ++lucas_ptr;
        }
        /* Store current as next d value */
        grow_seq_t_array(&d_cache, &d_cap, d_len + 1);
        d_cache[d_len++] = (seq_t)cur;
        ++cur;
    }

    seq_t d = d_cache[(size_t)n];

    /* ---- Final value ---- */
    return b + d;
}
