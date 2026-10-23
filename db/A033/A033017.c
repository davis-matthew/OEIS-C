/*
 * A033017  Numbers whose base-4 expansion has no run of digits with length <
 *          2.
 * https://oeis.org/A033017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 507, u32 n <= 1600, u64 n <= 1600,
 *            u128 n <= 1600, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) for OEIS A033017: numbers whose base‑4 expansion has
 * no run of a single digit. The algorithm generates the base‑4 numbers
 * in increasing order that satisfy the run restriction and keeps
 * them cached for subsequent calls. */
#include <stddef.h>

#define CACHE_CAP 20000

static seq_t seq_cache[CACHE_CAP];
static size_t seq_cache_len = 0;
static idx_t seq_cur_len = 2;

static void rec(idx_t pos, idx_t len, int last_digit, int run_len,
                seq_t value) {
    if (seq_cache_len >= CACHE_CAP) return;          /* safety cap */

    if (pos == len - 1) {
        /* finish the run – only the same digit keeps validity */
        seq_t val = value * ((seq_t)4) + (seq_t)last_digit;
        seq_cache[seq_cache_len++] = val;
        return;
    }

    for (int d = 0; d <= 3; ++d) {
        if (d == last_digit) {
            rec(pos + 1, len, last_digit, run_len + 1,
                value * ((seq_t)4) + (seq_t)d);
        } else if (run_len >= 2) {
            rec(pos + 1, len, d, 1,
                value * ((seq_t)4) + (seq_t)d);
        }
        if (seq_cache_len >= CACHE_CAP) return;
    }
}

static void generate_len(idx_t len) {
    for (int d = 1; d <= 3; ++d) {
        rec(1, len, d, 1, (seq_t)d);
        if (seq_cache_len >= CACHE_CAP) break;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;

    while (seq_cache_len < (size_t)n) {
        generate_len(seq_cur_len);
        seq_cur_len++;
    }
    return seq_cache[n - 1];
}
