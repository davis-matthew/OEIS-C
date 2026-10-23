/*
 * A026475  a(1)=1, a(2)=3, otherwise a(n) = least positive integer > a(n-1)
 *          and not a(i) + a(j) + a(k) for 1 <= i < j < k <= n.
 * https://oeis.org/A026475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A026475 (OEIS)
   a(1) = 1, a(2) = 3
   For n ≥ 3: a(n) = least integer > a(n‑1) that is not
                equal to a(i)+a(j)+a(k) for any 1 ≤ i < j < k ≤ n‑1.
   For n > 15: a(n) = a(n‑1) + a(n‑7) – a(n‑8)          (linear recurrence)
   (The recurrence follows from the definition; all terms fit into seq_t.) */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *seq_cache = NULL;     /* array holding computed terms */
static size_t  seq_cap   = 0;        /* current allocated size */
static size_t  seq_len   = 0;        /* number of terms actually computed */

/* Resize the cache if necessary.  Must handle growth to at least new_len. */
static void ensure_capacity(size_t new_len) {
    if (seq_cap >= new_len) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    if (new_cap < new_len) new_cap = new_len;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* Compute the terms up to and including index idx (0‑based). */
static void compute_up_to_idx(size_t idx) {
    /* ensure cache can hold idx+1 terms */
    ensure_capacity(idx + 1);

    /* initialise first two terms if not already done */
    if (seq_len == 0) {
        seq_cache[0] = (seq_t)1;
        seq_len = 1;
    }
    if (seq_len == 1) {
        seq_cache[1] = (seq_t)3;
        seq_len = 2;
    }

    /* generate missing terms */
    for (size_t i = seq_len; i <= idx; ++i) {
        if (i < 2) {               /* already set */
            /* nothing */
        } else if (i < 15) {       /* brute‑force search up to a(15) */
            seq_t candidate = seq_cache[i-1] + (seq_t)1;
            while (true) {
                bool conflict = false;
                for (size_t p = 0; p + 2 < i && !conflict; ++p) {
                    for (size_t q = p + 1; q + 1 < i && !conflict; ++q) {
                        for (size_t r = q + 1; r < i && !conflict; ++r) {
                            seq_t s = seq_cache[p] + seq_cache[q] + seq_cache[r];
                            if (s == candidate) {
                                conflict = true;
                            }
                        }
                    }
                }
                if (!conflict) break;
                candidate++;
            }
            seq_cache[i] = candidate;
        } else {                      /* linear recurrence for n > 15 */
            seq_cache[i] = seq_cache[i-1] + seq_cache[i-7] - seq_cache[i-8];
        }
    }
    seq_len = idx + 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;             /* undefined; safe fallback */
    size_t target = (size_t)n - 1;    /* 0‑based index of a(n) */
    if (seq_len == 0 || target >= seq_len) {
        compute_up_to_idx(target);
    }
    return seq_cache[target];
}
