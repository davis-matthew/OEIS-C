/*
 * A184155  The Matula-Goebel number of rooted trees having all leaves at the
 *          same level.
 * https://oeis.org/A184155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   seq_push_back
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The A184155 sequence: Matula-Goebel numbers of rooted trees
 * where all leaves are at the same depth.  The function below
 * generates all such numbers in increasing order by testing each
 * integer n with a sieve of smallest prime factors.  The n‑th term
 * is returned from a dynamically built sorted list of valid
 * numbers. */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_push_back.h"

/* Static state variables used by the sequence generator  */
static seq_t *seq_arr      = NULL;   /* sorted list of valid numbers */
static size_t seq_len      = 0;      /* current length of seq_arr      */
static size_t seq_cap      = 0;      /* capacity of seq_arr            */
static idx_t  current_lim  = 0;      /* last limit used to generate seq_arr */

/* --------------------------------------------------------------------- */
/* Append a value to a dynamic array.  Grow the array as needed.         */

/* --------------------------------------------------------------------- */
/* Build the list of valid Matula-Goebel numbers up to 'lim' inclusive.  */
/* This function resets seq_arr, seq_len and seq_cap each time it is     */
/* called (the old array is freed, a new one is created).                */
/* --------------------------------------------------------------------- */
static void
build_sequence(idx_t lim)
{
    /* Free previous data (if any) */
    if (seq_arr) {
        free(seq_arr);
        seq_arr   = NULL;
        seq_cap   = 0;
        seq_len   = 0;
    }

    /* ---------- Allocate helper arrays ---------- */
    uint32_t *spf      = (uint32_t *)calloc((size_t)lim + 1, sizeof(uint32_t)); /* smallest prime factor */
    uint32_t *primeIdx = (uint32_t *)calloc((size_t)lim + 1, sizeof(uint32_t)); /* 1‑based prime index, 0 if composite */
    /* Approximate number of primes: lim / log(lim) + 10   */
    size_t primes_cap = (size_t)(lim / 10 + 10);
    idx_t *primes     = (idx_t *)malloc(primes_cap * sizeof(idx_t));
    size_t pcnt       = 0;

    /* ---------- Linear sieve to fill spf and primeIdx ---------- */
    spf[0] = 0;  spf[1] = 1;
    for (idx_t i = 2; i <= lim; ++i) {
        if (!spf[i]) {                       /* i is prime */
            spf[i] = (uint32_t)i;
            if (pcnt >= primes_cap) {         /* grow primes array if needed */
                primes_cap <<= 1;
                primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
            }
            primes[pcnt++] = i;
            primeIdx[i] = (uint32_t)(pcnt); /* 1‑based index of prime "i" */
        }
        for (size_t j = 0; j < pcnt; ++j) {
            idx_t p = primes[j];
            if (p > spf[i] || (unsigned long long)i * p > lim) break;
            spf[(idx_t)(i * p)] = p;
        }
    }

    /* ---------- Arrays to store uniformity flags & leaf depth ---------- */
    uint8_t *uniform = (uint8_t *)calloc((size_t)lim + 1, sizeof(uint8_t));
    idx_t   *depth   = (idx_t *)calloc((size_t)lim + 1, sizeof(idx_t));

    /* ---------- Build the sorted list of valid numbers ---------- */
    /* n = 1 is always valid: tree consisting of a single node */
    seq_push_back(&seq_arr, &seq_cap, &seq_len, (seq_t)1);
    uniform[1] = 1;
    depth[1]   = 0;

    for (idx_t n = 2; n <= lim; ++n) {
        idx_t tmp = n;
        bool ok = true;
        idx_t leafDepth = (idx_t)(-1);   /* depth of leaves in child subtrees */

        while (tmp > 1) {
            idx_t p = spf[tmp];

            /* t is the Matula number of the subtree rooted at this child */
            idx_t t = (idx_t)primeIdx[p];
            if (!uniform[t]) {           /* subtree not uniform -> reject */
                ok = false;
                break;
            }

            if (leafDepth == (idx_t)(-1))
                leafDepth = depth[t];
            else if (leafDepth != depth[t]) { /* different leaf depths -> reject */
                ok = false;
                break;
            }

            /* skip all copies of this prime (multiplicity irrelevant) */
            while (tmp % p == 0) tmp /= p;
        }

        if (ok) {                 /* n is valid */
            uniform[n] = 1;
            depth[n]   = leafDepth + 1;
            seq_push_back(&seq_arr, &seq_cap, &seq_len, (seq_t)n);
        } else {
            uniform[n] = 0;
        }
    }

    /* ---------- Clean up intermediate tables ---------- */
    free(spf);
    free(primeIdx);
    free(primes);
    free(uniform);
    free(depth);
}

/* --------------------------------------------------------------------- */
/* Make sure that seq_arr contains at least the first 'n' terms.         */
/* If not, increase the range and rebuild the table.                    */
/* --------------------------------------------------------------------- */
static void
ensure(idx_t n)
{
    if (seq_len >= n) return;   /* already enough terms */

    /* Start with an initial range; grow until we meet the requirement. */
    idx_t lim = (current_lim > 0) ? current_lim : 10000;
    while (seq_len < n) {
        build_sequence(lim);
        if (seq_len >= n) {
            current_lim = lim;
            break;
        }
        lim <<= 1;          /* double the limit for next attempt */
    }
}

/* --------------------------------------------------------------------- */
/* The function required by the build system.                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure(n);
    return seq_arr[n - 1];   /* a(1) is the first term in seq_arr */
}
