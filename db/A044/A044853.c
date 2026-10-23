/*
 * A044853  Positive integers having no fewer base-12 runs of even length than
 *          odd.
 * https://oeis.org/A044853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044853: Positive integers having no fewer base‑12 runs of 
   even length than odd.  The returned value is the n‑th such integer. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Return true if the base‑12 representation of x has at least as many
   even‑length runs as odd‑length runs. */
static bool even_runs_at_least_odd(seq_t x)
{
    seq_t prev = x % 12;        /* first digit (least significant)    */
    seq_t run_len = 1;          /* current run length                 */
    seq_t even = 0, odd = 0;    /* counters for runs                  */

    x /= 12;
    while (x) {
        seq_t d = x % 12;
        x /= 12;
        if (d == prev) {
            run_len++;
        } else {
            if (run_len & 1) odd++; else even++;
            prev = d;
            run_len = 1;
        }
    }
    /* record the final run */
    if (run_len & 1) odd++; else even++;

    return even >= odd;
}

/* Cached results of a(n) for repeated calls. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* a(n) – n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1; 0 is unused        */
    /* Expand cache until we have at least n entries. */
    if (n > cache_len) {
        seq_t cur = (cache_len ? cache[cache_len - 1] + 1 : (seq_t)1);
        size_t cap = (cache_len ? cache_len * 2 : 64);
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        while (cache_len < n) {
            if (even_runs_at_least_odd(cur)) {
                if (cache_len >= cap) {
                    cap *= 2;
                    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                }
                cache[cache_len] = cur;
                cache_len++;
            }
            cur++;
        }
    }
    return cache[n - 1];
}
