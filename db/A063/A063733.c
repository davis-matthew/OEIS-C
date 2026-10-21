/*
 * A063733  A variant of Recamán's sequence: a(0)=1, a(n) = a(n-1)-(n-1) if
 *          positive and new, else a(n) = a(n-1)+(n-1).
 * https://oeis.org/A063733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, look
 * Formula:   a(n) = A005132(n‑1) – 1 Recurrence: a(0)=1; for n>0, let d=n‑1;
 *            if a(n‑1)–d >0 and not yet seen, a(n)=a(n‑1)–d; otherwise
 *            a(n)=a(n‑1)+d
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Cached sequence values. */
static seq_t *seq = NULL;
static size_t seq_len = 0;      /* number of terms stored */
static size_t seq_cap = 0;      /* allocated capacity */

/* Visited flag array. */
static uint8_t *visited = NULL;
static size_t visited_cap = 0;   /* allocated capacity */

/* Ensure seq has space for at least `need` elements. */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 1;
    if (new_cap < need) new_cap = need;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure visited has space for indices up to `need`. */
static void ensure_visited_cap(size_t need)
{
    if (visited_cap > need) return;   /* need < visited_cap is safe */
    size_t new_cap = visited_cap ? visited_cap << 1 : 1;
    while (new_cap <= need) new_cap <<= 1;
    visited = (uint8_t *)realloc(visited, new_cap * sizeof(uint8_t));
    memset(visited + visited_cap, 0, (new_cap - visited_cap) * sizeof(uint8_t));
    visited_cap = new_cap;
}

/* External linkage */
seq_t a(idx_t n)
{
    /* Initialization on first call. */
    if (!seq) {
        seq_cap = 1;
        seq = (seq_t *)malloc(sizeof(seq_t));
        seq[0] = (seq_t)1;          /* a(0) = 1 */
        seq_len = 1;
    }
    if (!visited) {
        visited_cap = 2;            /* indices 0..1 */
        visited = (uint8_t *)calloc(visited_cap, sizeof(uint8_t));
        visited[1] = 1;             /* value 1 has appeared */
    }

    /* Compute up to requested index. */
    if (n >= seq_len) {
        size_t up_to = (size_t)n;
        ensure_seq_cap(up_to + 1);
        for (size_t k = seq_len; k <= up_to; ++k) {
            seq_t prev  = seq[k - 1];
            seq_t delta = (seq_t)(k - 1);
            seq_t cand  = prev > delta ? prev - delta : 0;

            bool is_new = false;
            if (cand > 0) {
                if (cand >= (seq_t)visited_cap) {
                    ensure_visited_cap((size_t)cand);
                }
                if (!visited[(size_t)cand]) {
                    is_new = true;
                }
            }

            if (!is_new) {
                cand = prev + delta;
            }

            seq[k] = cand;
            seq_len = k + 1;

            if (cand >= (seq_t)visited_cap) {
                ensure_visited_cap((size_t)cand);
            }
            visited[(size_t)cand] = 1;
        }
    }
    return seq[(size_t)n];
}
