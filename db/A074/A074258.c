/*
 * A074258  Gaps between primes p such that 2p-1 is also prime.
 * https://oeis.org/A074258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   gaps between successive primes p such that 2p-1 is also prime.
 *            The sequence is 1-based; a(1) is the first gap after the
 *            smallest qualifying prime (p=2). The implementation generates
 *            qualifying primes on the fly, stores their gaps in a dynamically
 *            resizable table, and returns table[n-1] for any n≥1
 * Range:     u8 n <= 237, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrime64.h"   /* isPrime64(uint64_t) – fast 64‑bit primality test */

/* ------------------------------------------------------------------ */
/* Dynamic gap storage (static across calls) */

static seq_t *gap_table   = NULL;   /* array of gaps (each stored as seq_t)     */
static size_t gap_capacity = 0;   /* allocated slots in gap_table             */
static size_t gap_count    = 0;   /* number of gaps currently stored          */

/* ------------------------------------------------------------------ */
/* Generation state */

static idx_t next_p          = 2;  /* next integer to test for primality      */
static idx_t last_special    = 0;  /* previous special prime (p where 2p-1 prime) */

/* ------------------------------------------------------------------ */
/* Ensure that the gap table can hold at least 'need' entries.
   If currently too small, it is grown (doubling strategy). */

static void ensure_gap_capacity(size_t need)
{
    if (gap_capacity >= need) return;

    size_t new_cap = (gap_capacity == 0) ? 128 : gap_capacity * 2;
    if (new_cap < need) new_cap = need;

    seq_t *new_table = (seq_t *)realloc(gap_table, new_cap * sizeof(seq_t));
    if (!new_table) {                 /* allocation failure – abort silently */
        exit(1);
    }
    gap_table   = new_table;
    gap_capacity = new_cap;
}

/* ------------------------------------------------------------------ */
/* Generate qualifying primes until at least 'n' gaps are available. */

static void generate_gaps_until(size_t n)
{
    while (gap_count < n) {
        /* Find next prime p */
        for (;;) {
            if (isPrime64(next_p)) break;
            ++next_p;
        }

        idx_t p = next_p;
        ++next_p;                         /* keep testing future candidates   */

        /* Check whether 2p-1 is prime.  Use 128‑bit to avoid overflow. */
        idx_t q = (idx_t)(((unsigned __int128)p << 1) - 1);
        if (!isPrime64(q)) continue;       /* not a qualifying prime          */

        /* Record the gap if we already have a preceding special prime. */
        if (last_special != 0) {
            ensure_gap_capacity(gap_count + 1);
            gap_table[gap_count++] = (seq_t)(p - last_special);
        }
        last_special = p;                   /* update previous qualifying prime */
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the challenge. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for indices < 1 */
    generate_gaps_until((size_t)n);
    return gap_table[n - 1];          /* 1‑based indexing */
}
