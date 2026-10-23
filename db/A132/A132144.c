/*
 * A132144  Numbers that can't be expressed as the sum of a prime number and a
 *          Fibonacci number (0 is considered to be a Fibonacci number).
 * https://oeis.org/A132144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer that cannot be expressed as *
 *            the sum of a prime number and a Fibonacci number (0 is
 *            considered a * Fibonacci number)
 * Range:     u8 n <= 9, u16 n <= 5743, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

static seq_t *g_fibs     = NULL;   /* array of Fibonacci numbers */
static size_t g_fibs_len = 0;      /* number of Fibonacci numbers stored */
static size_t g_fibs_cap = 0;      /* allocated capacity */

/* Ensure that g_fibs contains every Fibonacci number <= limit.  */
static void ensure_fibs_up_to(seq_t limit)
{
    /* Initialise on first use.  */
    if (!g_fibs) {
        g_fibs_cap = 8;
        g_fibs = (seq_t *)malloc(g_fibs_cap * sizeof(seq_t));
        if (!g_fibs) exit(EXIT_FAILURE);
        g_fibs[0] = (seq_t)0;
        g_fibs[1] = (seq_t)1;
        g_fibs_len = 2;
    }

    /* Grow capacity if needed.  */
    while (g_fibs_len >= g_fibs_cap) {
        size_t new_cap = g_fibs_cap * 2;
        seq_t *tmp = (seq_t *)realloc(g_fibs, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        g_fibs = tmp;
        g_fibs_cap = new_cap;
    }

    /* Append Fibonacci numbers until the last one is > limit.  */
    while (g_fibs_len < 2 || g_fibs[g_fibs_len - 1] <= limit) {
        seq_t a = g_fibs[g_fibs_len - 1];
        seq_t b = g_fibs[g_fibs_len - 2];
        seq_t next = a + b;

        /* Check for overflow (in the fixed‑width case).  */
        if (next < a) break;

        /* Ensure we still have room for the new element.  */
        if (g_fibs_len >= g_fibs_cap) {
            size_t new_cap = g_fibs_cap * 2;
            seq_t *tmp = (seq_t *)realloc(g_fibs, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            g_fibs = tmp;
            g_fibs_cap = new_cap;
        }

        g_fibs[g_fibs_len++] = next;
    }
}

/* Sequence A132144.  */
seq_t a(idx_t n)
{
    idx_t found = 0;          /* how many non‑representable numbers found */
    seq_t cand  = (seq_t)1;   /* current candidate integer */

    for (;;) {
        ensure_fibs_up_to(cand);

        bool repr = false;    /* can cand be written as p + F? */

        /* Try every Fibonacci number <= cand.  */
        for (size_t i = 0; i < g_fibs_len && g_fibs[i] <= cand; ++i) {
            seq_t diff = cand - g_fibs[i];
            if (diff < (seq_t)2) continue;          /* primes start at 2 */

            /* Prime test: convert the diff to idx_t.  We rely on the
               fact that all terms needed for n up to about 10000
               fit in 64 bits, which is within idx_t. */
            if (is_prime((idx_t)diff)) {
                repr = true;
                break;
            }
        }

        if (!repr) {
            ++found;
            if (found == n)
                return cand;
        }
        ++cand;
    }
}
