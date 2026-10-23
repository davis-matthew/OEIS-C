/*
 * A033016  Numbers whose base-3 expansion has no run of digits with length <
 *          2.
 * https://oeis.org/A033016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 682, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description: a(n) is the n-th integer whose base‑3 representation
 * consists of digit runs of length at least two. Numbers are generated
 * in ascending order and cached for repeated calls. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t *seq_cache = NULL;     /* array of found numbers   */
static size_t cache_cap = 0;        /* current allocated size   */
static size_t cache_len = 0;        /* number of numbers stored */
static idx_t  next_val   = 1;       /* next integer to test     */

/* test whether every run of digits in the ternary expansion of x
 * has length >= 2. */
static bool valid_ternary(seq_t x)
{
    int last_digit = -1;
    int run_len    = 0;
    const seq_t three = (seq_t)3;

    while (x > (seq_t)0) {
        int cur = (int)(x % three);

        if (last_digit == -1) {          /* first digit seen */
            last_digit = cur;
            run_len    = 1;
        } else if (cur == last_digit) {
            run_len++;                    /* continue current run */
        } else {
            if (run_len == 1) return false; /* run too short */
            last_digit = cur;
            run_len    = 1;
        }
        x /= three;
    }
    /* final run must also be of length >= 2 */
    return run_len != 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1; return 0 */

    /* gather numbers until we have at least n of them */
    while (cache_len < n) {
        /* find next integer with the required property */
        while (!valid_ternary((seq_t)next_val))
            next_val++;

        /* ensure the array has enough space */
        if (cache_len == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 1024;
            seq_t *tmp = (seq_t *)realloc(seq_cache,
                                         cache_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;          /* allocation failed */
            seq_cache = tmp;
        }

        seq_cache[cache_len++] = (seq_t)next_val;
        next_val++;
    }

    return seq_cache[n - 1];
}
