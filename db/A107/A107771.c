/*
 * A107771  Numbers n such that 2*n + 5 and 5*n + 2 are primes.
 * https://oeis.org/A107771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 2284, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based: numbers n such that 2*n + 5 and 5*n + 2 are primes */
/*    (a(1) = 0, a(2) = 1, a(3) = 3, …) */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* provides: bool is_prime(uint64_t n) */

static seq_t *seq = NULL;
static idx_t  seq_cap = 0;
static idx_t  seq_len = 0;
static idx_t  last_checked = 0;   /* next n to test */

/* Ensure the internal array has room for at least *min_needed elements */
static void _ensure_capacity(idx_t min_needed) {
    if (seq_cap >= min_needed) return;
    while (seq_cap < min_needed) {
        seq_cap = (seq_cap == 0) ? 64 : seq_cap << 1;
    }
    seq = (seq_t *)realloc(seq, sizeof(seq_t) * seq_cap);
}

/* Generate terms up to the given index (1‑based). */
static void _grow_to(idx_t need_index) {
    if (seq_len >= need_index) return;
    while (seq_len < need_index) {
        uint64_t n = (uint64_t)last_checked;
        uint64_t a = 2ULL * n + 5ULL;
        uint64_t b = 5ULL * n + 2ULL;
        if (is_prime(a) && is_prime(b)) {
            _ensure_capacity(seq_len + 1);
            seq[seq_len++] = (seq_t)n;
        }
        ++last_checked;
    }
}

seq_t a(idx_t n) {
    /* n is 1‑based; a(1) = 0 */
    if (n == 0) return 0;  /* undefined but harmless */
    _grow_to(n);
    return seq[n - 1];
}
