/*
 * A048304  Numbers whose base-7 expansions, read from left to right, have run
 *          lengths that strictly increase.
 * https://oeis.org/A048304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search of numbers whose base‑7 expansion has strictly increasing run lengths */
#include <stdlib.h>
#include <stdbool.h>

static bool good(seq_t x) {
    if (x == 0)
        return true;
    seq_t t = x;
    unsigned char dig[200];
    size_t len = 0;
    while (t > 0) {
        dig[len++] = (unsigned char)(t % 7);
        t = t / 7;
    }
    seq_t prev_run = 0;
    unsigned char prev_digit = dig[len - 1];
    seq_t run_len = 1;
    for (size_t i = len - 1; i > 0; --i) {
        unsigned char d = dig[i - 1];
        if (d == prev_digit) {
            run_len++;
        } else {
            if (run_len <= prev_run)
                return false;
            prev_run = run_len;
            prev_digit = d;
            run_len = 1;
        }
    }
    return run_len > prev_run;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;
    static seq_t next_candidate = 0;

    while (cache_size <= n) {
        if (cache_size == cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }
        while (!good(next_candidate))
            ++next_candidate;
        cache[cache_size++] = next_candidate;
        ++next_candidate;
    }
    return cache[n];
}
