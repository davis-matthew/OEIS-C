/*
 * A181097  Permutation of positive integers created from sequence A002808(n)
 *          = composites.
 * https://oeis.org/A181097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: build the permutation by repeatedly swapping the
   smallest unused index (vacancy) with the next composite number
   in ascending order.  This implements OEIS A181097. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ---- Global state (static) ------------------------------------ */
static seq_t *perm          = NULL;   /* permutation array, 1‑based */
static size_t permSize      = 0;      /* current size of perm */
static idx_t  nextVacancy   = 1;      /* next candidate vacancy index */
static uint64_t nextComposite = 4;    /* next composite candidate */

/* ---- Utility functions ---------------------------------------- */

/* Ensure perm has room for indices up to `needed-1`. */
static void ensure_perm_size(size_t needed)
{
    if (needed <= permSize) return;

    size_t newSize = permSize > 0 ? permSize << 1 : 16;
    while (newSize < needed) newSize <<= 1;

    seq_t *newPerm = (seq_t *)realloc(perm, newSize * sizeof(seq_t));
    if (!newPerm) exit(1);               /* allocation failure */

    /* Zero‑initialize the newly allocated part. */
    memset(newPerm + permSize, 0, (newSize - permSize) * sizeof(seq_t));

    perm   = newPerm;
    permSize = newSize;
}

/* Simple trial‑division primality test for 64‑bit values. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)          return false;
    if (n % 2 == 0)     return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Return next composite number (skipping primes) and advance counter. */
static uint64_t next_composite(void)
{
    while (true) {
        uint64_t c = nextComposite;
        ++nextComposite;
        if (!is_prime_uint64(c))
            return c;
    }
}

/* ---- Main sequence function ----------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is offset 1 */

    /* Initialise storage if first call. */
    if (perm == NULL) {
        permSize = 16;
        perm = (seq_t *)calloc(permSize, sizeof(seq_t));
    }

    /* Make array large enough for index n. */
    ensure_perm_size((size_t)n + 1);

    /* Process composites until all indices ≤ n are filled. */
    while (nextVacancy <= n) {
        /* Find the next vacant slot. */
        while (nextVacancy <= n && perm[(size_t)nextVacancy] != 0)
            ++nextVacancy;
        if (nextVacancy > n) break;

        /* Get the next composite number. */
        uint64_t c = next_composite();

        /* Ensure we have space for both the vacancy and the composite. */
        size_t needed = c > (uint64_t)nextVacancy ? c + 1 : nextVacancy + 1;
        ensure_perm_size(needed);

        /* Assign values as per the swapping rule. */
        perm[(size_t)nextVacancy] = (seq_t)c;     /* vacancy gets composite */
        perm[(size_t)c]          = (seq_t)nextVacancy;  /* composite gets vacancy */

        /* Prepare for the next iteration. */
        ++nextVacancy;
    }

    return perm[(size_t)n];
}
