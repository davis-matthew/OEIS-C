/*
 * A193408  Hill numbers.
 * https://oeis.org/A193408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth smallest integer whose decimal digits form a
 *            hill: first and last digit are equal (1‑9), the digits up to the
 *            maximum are nondecreasing, the digits after the maximum are
 *            nonincreasing. This is implemented by generating all such
 *            numbers in ascending order using a recursive construction
 * Range:     u8 n <= 31, u16 n <= 1753, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t *hill = NULL;
static size_t hill_cap = 0;
static size_t hill_len = 0;
static idx_t max_len_generated = 0;
static bool initialized = false;

static void ensure_hill_capacity(size_t needed)
{
    if (needed > hill_cap) {
        size_t new_cap = hill_cap == 0 ? 64 : hill_cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *new_arr = (seq_t *)realloc(hill, new_cap * sizeof(seq_t));
        if (!new_arr) abort();
        hill = new_arr;
        hill_cap = new_cap;
    }
}

static void generate_length_rec(idx_t pos, idx_t len, seq_t val,
                                int prev, int first, bool dec)
{
    if (pos == len - 1) {
        int d = first;
        if (!dec && d < prev) dec = true;
        if (dec && d > prev) return;
        if (!dec && d < prev) return;
        ensure_hill_capacity(hill_len + 1);
        hill[hill_len++] = val * 10 + d;
        return;
    }
    for (int d = 1; d <= 9; ++d) {
        bool new_dec = dec;
        if (!dec) {
            if (d < prev) new_dec = true;
        } else {
            if (d > prev) continue;
        }
        generate_length_rec(pos + 1, len, val * 10 + d, d, first, new_dec);
    }
}

static void generate_length(idx_t len)
{
    for (int f = 1; f <= 9; ++f) {
        if (len == 1) {
            ensure_hill_capacity(hill_len + 1);
            hill[hill_len++] = f;
        } else {
            seq_t init_val = f;
            generate_length_rec(1, len, init_val, f, f, false);
        }
    }
}

static void extend_up_to(idx_t n)
{
    if (!initialized) {
        generate_length(1);
        max_len_generated = 1;
        initialized = true;
    }
    while (hill_len < n) {
        ++max_len_generated;
        generate_length(max_len_generated);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* out of offset range, define as 0 */
    extend_up_to(n);
    return hill[n - 1];
}
