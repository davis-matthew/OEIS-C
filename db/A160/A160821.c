/*
 * A160821  a(1)=2. a(n) = smallest positive integer such that omega(a(n)) =
 *          omega(sum{k=1 to n} a(k)), where omega(m) = A001221(m).
 * https://oeis.org/A160821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer m such that * ω(m) =
 *            ω(∑_{k=1}^n a(k)), with ω = A001221 (number of distinct primes)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/count_distinct_primes.h"

/* Global static storage: a(n) in 'a_seq' (seq_t type) and cumulative sums in
 * 'sum_seq' (uint64_t).  The arrays are 1‑based, index 0 unused. */
static size_t _max_n = 0;          /* highest n computed so far */
static size_t _cap = 0;            /* capacity of the arrays (≥ _max_n) */
static seq_t *a_seq = NULL;        /* a(n) values */
static uint64_t *sum_seq = NULL;   /* ∑_{k=1}^n a(k) as uint64_t */

/* Ensure that the arrays are large enough to hold elements up to n (inclusive). */
static void _ensure_capacity(size_t n)
{
    if (n <= _cap) return;
    size_t new_cap = _cap ? _cap * 2 : 64;
    while (new_cap < n) new_cap <<= 1;

    seq_t *new_a = (seq_t *)realloc(a_seq, (new_cap + 1) * sizeof(seq_t));
    uint64_t *new_s = (uint64_t *)realloc(sum_seq, (new_cap + 1) * sizeof(uint64_t));
    if (!new_a || !new_s) abort();   /* allocation failure */

    a_seq = new_a;
    sum_seq = new_s;
    _cap = new_cap;
}

/* Build the sequence up to and including n, if needed. */
static void _build_upto(idx_t n)
{
    if (n <= (idx_t)_max_n) return;
    _ensure_capacity((size_t)n);

    /* Initialise first term if not already done. */
    if (_max_n == 0) {
        a_seq[1] = (seq_t)2;
        sum_seq[1] = (uint64_t)2;
        _max_n = 1;
    }

    for (idx_t cur = (idx_t)(_max_n + 1); cur <= n; ++cur) {
        uint64_t prev_sum = sum_seq[(size_t)cur - 1];
        uint64_t cand = 1;
        while (1) {
            uint64_t candidate = cand;          /* candidate value as uint64_t */

            /* Count distinct prime factors of candidate and of the new sum. */
            int omega_candidate = count_distinct_primes((uint64_t)candidate);
            int omega_sum = count_distinct_primes(prev_sum + candidate);

            if (omega_candidate == omega_sum) {
                a_seq[(size_t)cur]   = (seq_t)candidate;     /* store a(cur) */
                sum_seq[(size_t)cur] = prev_sum + candidate; /* update cumulative sum */
                break;
            }
            ++cand;
        }
    }
    _max_n = (size_t)n;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) abort();          /* undefined for n < 1 */
    _build_upto(n);
    return a_seq[(size_t)n];
}
