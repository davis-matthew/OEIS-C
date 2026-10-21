/*
 * A084035  a(1) = 1 and then distinct numbers such that the sum as well the
 *          absolute difference of successive terms is a prime.
 * https://oeis.org/A084035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   is_prime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A084035:
   a(1) = 1
   for n > 1, a(n) is the smallest unused integer such that
   both a(n-1)+a(n) and |a(n-1)-a(n)| are prime.
   The sequence is a permutation of the natural numbers.
   Parity alternates: a(2k) is even, a(2k+1) is odd.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/is_prime64.h"

/* ---------- globals (static ---------- */
static seq_t *seq   = NULL;   /* array of computed terms, 0‑based */
static size_t seq_len = 0;    /* number of terms computed */
static size_t seq_cap = 0;    /* capacity of seq array */

/* marks whether a natural number has already been used */
static uint8_t *used   = NULL;   /* 1 = used, 0 = unused */
static size_t used_cap = 0;      /* capacity of used array */

/* ---------- helpers ---------- */
/* ensure used array can index at least v (inclusive) */
static void ensure_used_size(uint64_t v)
{
    if (v >= used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 128;
        if (new_cap <= v) new_cap = v + 1;
        uint8_t *tmp = (uint8_t *)realloc(used, new_cap);
        if (!tmp) abort();
        if (new_cap > used_cap)
            memset(tmp + used_cap, 0, new_cap - used_cap);
        used = tmp;
        used_cap = new_cap;
    }
}

/* ensure seq array can hold at least n terms */
static void ensure_seq_cap(size_t n)
{
    if (n > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq = tmp;
        seq_cap = new_cap;
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined; return 0 for safety */

    /* compute terms up to n if needed */
    while (seq_len < (size_t)n) {
        /* initialise first term */
        if (seq_len == 0) {
            ensure_seq_cap(1);
            seq[0] = (seq_t)1;
            seq_len = 1;
            ensure_used_size(1);
            used[1] = 1;
            continue;
        }

        seq_t prev = seq[seq_len - 1];
        uint64_t next_index = (uint64_t)seq_len + 1;   /* 1‑based index of the next term */

        /* candidate must have parity matching the index: odd → odd, even → even */
        uint64_t cand = (next_index % 2 == 0) ? 2ULL : 1ULL;

        for (;;) {
            ensure_used_size(cand);

            if (!used[(size_t)cand]) {
                /* cast to 64‑bit for primality tests */
                uint64_t prev64 = (uint64_t)prev;
                uint64_t cand64 = cand;

                uint64_t sum = prev64 + cand64;
                uint64_t diff = (prev64 >= cand64) ? (prev64 - cand64) : (cand64 - prev64);

                if (is_prime64(sum) && is_prime64(diff)) {
                    /* accept this candidate */
                    used[(size_t)cand] = 1;
                    ensure_seq_cap(seq_len + 1);
                    seq[seq_len] = (seq_t)cand;   /* cast back to seq_t */
                    seq_len++;
                    break;
                }
            }
            cand += 2ULL;   /* keep the same parity */
        }
    }

    return seq[(size_t)n - 1];
}
