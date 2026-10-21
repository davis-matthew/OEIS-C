/*
 * A078180  a(n) = smallest integer that can't be written as p*a(i)+1, for any
 *          prime p and 0<i<n, where a(1)=1 and a(n) > a(n-1).
 * https://oeis.org/A078180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5259, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A078180: a(1)=1, a(n) is the smallest integer that cannot be
   expressed as p*a(i)+1 for any prime p and 0<i<n.  The function builds
   a sieve of representable numbers incrementally. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* use the provided helper for generating primes up to a limit            */
#include "../utils/build_primes.h"

/* ----------------------------------------------------------------------- */
/*  Global data                                                           */
/* ----------------------------------------------------------------------- */
#define MARK_LIMIT   800000U          /* marks up to this value               */
#define INIT_CAP     128U             /* initial allocation for seq array     */

static uint8_t *marked  = NULL;      /* 1 if representable, 0 otherwise      */
static seq_t  *seq     = NULL;       /* sequence values, 1‑based             */
static size_t  seq_cap = 0;          /* allocated capacity for seq           */
static size_t  used    = 0;          /* number of terms computed             */
static bool    init    = false;      /* one‑time initialization flag         */
static idx_t  *prime_list = NULL;    /* list of primes (ascending)           */
static size_t  prime_cnt  = 0;       /* number of primes stored              */

/* ----------------------------------------------------------------------- */
/*  Helpers                                                                */
/* ----------------------------------------------------------------------- */

/* enlarge the sequence array if needed                                 */
static void grow_seq(size_t new_cap)
{
    if (new_cap <= seq_cap)
        return;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* mark all numbers of the form p*a+1 that are ≤ MARK_LIMIT             */
static void mark_value(const seq_t *a)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        /* t = prime * a + 1; use seq_t arithmetic throughout           */
        seq_t t = (seq_t)prime_list[i] * (*a) + 1;
        if (t <= (seq_t)MARK_LIMIT) {
            marked[(size_t)t] = 1;
        } else {
            /* primes grow, product will only increase                         */
            break;
        }
    }
}

/* ----------------------------------------------------------------------- */
/*  Public function                                                        */
/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* guard against n==0 (undefined)                                  */
    if (n == 0)
        return 0;

    if (!init) {                         /* one‑time init                 */
        /* allocate sequence buffer                                  */
        seq_cap = INIT_CAP;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));

        /* allocate mark bitmap                                       */
        marked = (uint8_t *)calloc(MARK_LIMIT + 1U, 1U);

        /* get primes up to MARK_LIMIT                               */
        build_primes((idx_t)MARK_LIMIT, &prime_list, &prime_cnt);

        /* first term                                                  */
        seq[1] = (seq_t)1;
        used   = 1;
        mark_value(&seq[1]);

        init = true;
    }

    /* compute terms until we reach the requested index                */
    while (used < (size_t)n) {
        seq_t cand = seq[used] + (seq_t)1;          /* next integer          */

        /* skip representable numbers                                */
        while (cand <= (seq_t)MARK_LIMIT &&
               marked[(size_t)cand]) {
            cand += (seq_t)1;
        }

        /* append new term                                           */
        grow_seq(used + 2);                        /* +1 for new term       */
        seq[++used] = cand;

        /* mark numbers caused by the new term                       */
        mark_value(&cand);
    }

    return seq[(size_t)n];
}
