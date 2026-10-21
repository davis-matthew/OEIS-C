/*
 * A076402  Euler's totient of n-th perfect power.
 * https://oeis.org/A076402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000010(A001597(n)) (Euler's totient of the n‑th perfect
 *            power)
 * Range:     u8 n <= 1, u16 n <= 56, u32 n <= 2373, u64 n <= 10000, u128 none,
 *            big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/calc_phi.h"

/* Static storage for the list of perfect powers (1‑based). */
static seq_t *perfect_powers = NULL;   /* 1‑based, perfect_powers[1] = 1 */
static size_t  perfect_count  = 0;     /* number of stored perfect powers (>=1) */
static bool    built          = false; /* true after first build */

/* Comparison function for qsort, defined inline. */
static int seq_compare(const void *a, const void *b)
{
    seq_t v1 = *(const seq_t *)a;
    seq_t v2 = *(const seq_t *)b;
    return (v1 == v2) ? 0 : (v1 < v2 ? -1 : 1);
}

/* Build the list of perfect powers up to a fixed bound that
   guarantees at least 10 000 terms.  The bound 1e12 is safe
   for n ≤ 10 000. */
static void build_perfect_powers(void)
{
    if (built) return;
    built = true;

    const seq_t LIMIT = (seq_t)1000000000000ULL; /* 1e12 */

    /* Dynamic vector for collected values before sorting. */
    size_t veccap   = 1024;
    size_t veclen   = 0;
    seq_t *vec      = (seq_t *)malloc(veccap * sizeof(seq_t));
    if (!vec) exit(EXIT_FAILURE);

    /* Generate all powers base^exp with base ≥ 2, exp ≥ 2, value ≤ LIMIT. */
    for (uint64_t base = 2;
         (seq_t)base * (seq_t)base <= LIMIT; /* base^2 <= LIMIT */
         ++base)
    {
        seq_t val = (seq_t)base * (seq_t)base; /* start at exponent 2 */
        while (val <= LIMIT)
        {
            /* Append val to the vector, growing it if necessary. */
            if (veclen == veccap)
            {
                veccap <<= 1;                       /* double capacity */
                seq_t *newvec = (seq_t *)realloc(vec, veccap * sizeof(seq_t));
                if (!newvec)
                {
                    free(vec);
                    exit(EXIT_FAILURE);
                }
                vec = newvec;
            }
            vec[veclen++] = val;

            /* Next power: val * base.  Break on overflow or exceeding limit. */
            seq_t next = val * (seq_t)base;
            if (next <= val) break;           /* overflow for unsigned */
            if (next > LIMIT) break;
            val = next;
        }
    }

    /* Sort the vector and remove duplicates. */
    qsort(vec, veclen, sizeof(seq_t), seq_compare);

    /* Count unique entries. */
    size_t uniqcap   = veccap;
    size_t unqlen    = 0;
    seq_t *uniq      = (seq_t *)malloc(uniqcap * sizeof(seq_t));
    if (!uniq) { free(vec); exit(EXIT_FAILURE); }

    seq_t last = 0;
    bool first = true;
    for (size_t i = 0; i < veclen; ++i)
    {
        seq_t cur = vec[i];
        if (first || cur != last)
        {
            if (unqlen == uniqcap)
            {
                uniqcap <<= 1;
                seq_t *newuniq = (seq_t *)realloc(uniq, uniqcap * sizeof(seq_t));
                if (!newuniq) { free(vec); free(uniq); exit(EXIT_FAILURE); }
                uniq = newuniq;
            }
            uniq[unqlen++] = cur;
            last = cur;
            first = false;
        }
    }
    free(vec);

    /* Allocate final 1‑based array and fill. */
    perfect_count = unqlen + 1;                  /* include 1 */
    perfect_powers = (seq_t *)malloc(perfect_count * sizeof(seq_t));
    if (!perfect_powers) { free(uniq); exit(EXIT_FAILURE); }

    perfect_powers[1] = (seq_t)1;                /* first perfect power */
    for (size_t i = 0; i < unqlen; ++i)
        perfect_powers[i + 2] = uniq[i];

    free(uniq);
}

/* Public function implementing the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined but harmless */

    /* Build the cache if it hasn't been done yet. */
    build_perfect_powers();

    /* Return phi of the n-th perfect power. */
    seq_t pw = perfect_powers[(size_t)n];
    return calc_phi(pw);
}
