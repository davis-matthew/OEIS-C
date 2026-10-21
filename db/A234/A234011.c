/*
 * A234011  The sums of 2 consecutive odious numbers (A000069).
 * https://oeis.org/A234011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = A000069(n) + A000069(n+1) (offset 1)
 * Range:     u8 n <= 64, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for odious numbers (1‑based indices) */
static seq_t *odious = NULL;    /* odious[k-1] = A000069(k)  */
static size_t odious_cap = 0;   /* allocated capacity in elements       */
static size_t odious_len = 0;   /* number of elements currently stored  */
static uint64_t next_candidate = 1;  /* next integer to test */

/* Ensure we have at least n+1 odious numbers stored (0‑based indices 0…n).*/
static void ensure_odious(idx_t n)
{
    while (odious_len <= (size_t)n) {
        /* Grow the array if needed. */
        if (odious_len == odious_cap) {
            size_t new_cap = odious_cap ? odious_cap * 2 : 16;
            void *tmp = realloc(odious, new_cap * sizeof(seq_t));
            if (!tmp)
                exit(EXIT_FAILURE);
            odious = (seq_t*)tmp;
            odious_cap = new_cap;
        }

        /* Test next_candidate for odiousness (odd popcount). */
        if (__builtin_popcountll(next_candidate) & 1)
            odious[odious_len++] = (seq_t)next_candidate;
        next_candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n<1, return 0 */

    /* We need odious[n‑1] and odious[n] – i.e. up to index n (0‑based). */
    ensure_odious(n);

    return odious[n - 1] + odious[n];
}
