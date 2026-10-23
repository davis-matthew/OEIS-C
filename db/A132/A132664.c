/*
 * A132664  a(1)=1, a(2)=2, a(n) = a(n-1) + n if the minimal positive integer
 *          not yet in the sequence is greater than a(n-1), else a(n) = a(n-1)
 *          - 1.
 * https://oeis.org/A132664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 197, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A132664: a(1)=1, a(2)=2, a(n) = 
   a(n-1)+n  if the smallest positive integer not yet in the sequence 
   is greater than a(n-1); otherwise a(n)=a(n-1)-1. 
   The function memoises values and the set of seen integers. 
*/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* static state for memoisation */
static seq_t   *seq          = NULL;   /* 1‑based array of computed terms */
static size_t  seq_cap       = 0;      /* allocated size of seq array */
static uint8_t *visited      = NULL;   /* bitmap of seen values, 1‑based */
static idx_t   visited_cap   = 0;      /* size of the visited bitmap */
static idx_t   min_missing   = 0;      /* current minimal missing positive integer */
static bool    initialized   = false;  /* has initialisation been done? */

/* helper: ensure visited bitmap can index v.  Assumes v<UINT64_MAX. */
static void ensure_visited_capacity(idx_t v)
{
    while (v >= visited_cap) {
        idx_t old = visited_cap;
        visited_cap = (visited_cap == 0) ? 1 : visited_cap << 1;
        uint8_t *tmp = (uint8_t*)realloc(visited, visited_cap * sizeof(uint8_t));
        if (!tmp) exit(EXIT_FAILURE); /* allocation failure */
        memset(tmp + old, 0, visited_cap - old);
        visited = tmp;
    }
}

static void ensure_min_missing_capacity(void)
{
    while (min_missing >= visited_cap) {
        idx_t old = visited_cap;
        visited_cap = (visited_cap == 0) ? 1 : visited_cap << 1;
        uint8_t *tmp = (uint8_t*)realloc(visited, visited_cap * sizeof(uint8_t));
        if (!tmp) exit(EXIT_FAILURE);
        memset(tmp + old, 0, visited_cap - old);
        visited = tmp;
    }
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* out‑of‑range; unused */

    /* initialise static data on first call */
    if (!initialized) {
        seq_cap      = 3;                  /* indices 0,1,2 */
        seq          = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq[1] = 1;
        seq[2] = 2;

        visited_cap  = 4;                  /* indices 0‑3 */
        visited      = (uint8_t*)malloc(visited_cap * sizeof(uint8_t));
        memset(visited, 0, visited_cap);
        visited[1] = 1;
        visited[2] = 1;

        min_missing  = 3;
        initialized = true;
    }

    /* extend seq array if necessary */
    size_t old_cap = seq_cap;
    if ((size_t)n + 1 > seq_cap) {
        seq_cap = (size_t)n + 1;
        seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
        if (!seq) exit(EXIT_FAILURE);
    }

    size_t current_len = old_cap - 1;          /* last computed index before any new work */
    for (idx_t i = current_len + 1; i <= n; ++i) {
        seq_t prev    = seq[i - 1];
        seq_t new_val;
        if (min_missing > prev)
            new_val = prev + (seq_t)i;         /* add i */
        else
            new_val = prev - 1;               /* decrement */

        seq[i] = new_val;

        /* mark new_val as seen */
        idx_t val_idx = (idx_t)new_val;
        ensure_visited_capacity(val_idx);
        visited[val_idx] = 1;

        /* update minimal missing positive integer */
        ensure_min_missing_capacity();
        while (min_missing < visited_cap && visited[min_missing]) {
            ++min_missing;
            ensure_min_missing_capacity();
        }
    }

    return seq[n];
}
