/*
 * A043799  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 1 mod 8.
 * https://oeis.org/A043799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that the number of runs in the base‑3 representation
   of n is congruent to 1 modulo 8.
   The function returns the nth term (n≥1) in ascending order. */

#include <stdlib.h>
#include <stdint.h>

static int count_runs_base3(seq_t n)
{
    /* Count the number of runs in the base‑3 representation of n. */
    if (n == 0) return 1;      /* base‑3 of 0 is "0": one run, but n≥1 for the sequence */
    int runs = 0;
    int prev = -1;             /* previous digit, -1 indicates none yet */
    seq_t x = n;
    const seq_t three = (seq_t)3;
    while (x > 0) {
        seq_t d = x % three;
        int dig = (int)d;
        if (dig != prev) {
            runs++;
            prev = dig;
        }
        x = x / three;
    }
    return runs;
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t candidate = 1;    /* next number to examine */

static void ensure_terms(idx_t n)
{
    while (cache_len < n) {
        if (count_runs_base3(candidate) % 8 == 1) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) abort();          /* allocation failed */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }
        candidate = candidate + (seq_t)1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, but return 0 for safety */
    ensure_terms(n);
    return cache[n - 1];
}
