/*
 * A176193  The positions of semiprimes in A002808.
 * https://oeis.org/A176193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001358(n) - A000720(A001358(n)) - 1 (positions of
 *            semiprimes in the list of composite numbers)
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

static size_t current_limit = 0;   /* maximum number considered in the sieve   */
static size_t sem_count    = 0;     /* number of semiprime positions found     */
static seq_t *sem_positions = NULL;/* array of positions of semiprimes        */

static void build_tables_to(size_t limit);
static void maybe_rebuild(idx_t n);

/* -------------------------------------------------------------------- */
static void build_tables_to(size_t limit)
{
    /* --------------------------------------------------------------- */
    /* Allocate and fill the smallest‑prime‑factor array for [0..limit] */
    /* --------------------------------------------------------------- */
    uint32_t *spf = (uint32_t *)calloc(limit + 1U, sizeof(uint32_t));
    if (!spf) return;          /* allocation failure – leave tables empty   */

    for (size_t i = 2; i <= limit; ++i)
    {
        if (spf[i] == 0U)      /* i is prime                           */
        {
            spf[i] = (uint32_t)i;
            if (i * i <= limit)
            {
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0U)
                        spf[j] = (uint32_t)i;
            }
        }
    }

    /* --------------------------------------------------------------- */
    /* Allocate space for observed semiprime positions                 */
    /* --------------------------------------------------------------- */
    seq_t *new_positions = alloc_seq(limit);
    if (!new_positions)
    {
        free(spf);
        return;
    }

    size_t comp_idx = 0;          /* 1‑based index among composites          */
    size_t new_sem_count = 0;

    /* --------------------------------------------------------------- */
    /* Walk through all numbers, identify composites and semiprimes   */
    /* --------------------------------------------------------------- */
    for (size_t i = 4; i <= limit; ++i)
    {
        if (spf[i] != (uint32_t)i)          /* composite         */
        {
            ++comp_idx;

            /* Count prime factors (with multiplicity) using spf          */
            size_t x = i;
            int cnt = 0;
            while (x > 1U && cnt < 3)       /* stop early if >2 factors          */
            {
                uint32_t p = spf[x];
                ++cnt;
                x /= p;
            }

            /* Exactly two prime factors AND product reached 1 → semiprime */
            if (cnt == 2 && x == 1U)
                new_positions[new_sem_count++] = (seq_t)comp_idx;
        }
    }

    /* --------------------------------------------------------------- */
    /* Replace old tables with the new ones                           */
    /* --------------------------------------------------------------- */
    if (sem_positions)
        free_seq(sem_positions);

    sem_positions = new_positions;
    sem_count    = new_sem_count;
    current_limit = limit;

    free(spf);
}

/* -------------------------------------------------------------------- */
static void maybe_rebuild(idx_t n)
{
    /* Initial construction ------------------------------------------- */
    if (current_limit == 0)
    {
        /* Start with a reasonable bound that covers most queries up to 10 000 */
        build_tables_to(200000U);

        /* If still not enough, double the limit until we have n semiprimes */
        size_t new_limit = current_limit * 2U;
        while (n > sem_count)
        {
            build_tables_to(new_limit);
            new_limit *= 2U;
        }
    }
    else if (n > sem_count)
    {
        /* Current tables insufficient – extend by doubling until enough   */
        size_t new_limit = current_limit;
        while (n > sem_count)
        {
            new_limit *= 2U;
            build_tables_to(new_limit);
        }
    }
}

/* -------------------------------------------------------------------- */
/* Compute the nth term of A176193                                      */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n = 0, return 0 */

    maybe_rebuild(n);
    return sem_positions[(size_t)(n - 1U)];
}
