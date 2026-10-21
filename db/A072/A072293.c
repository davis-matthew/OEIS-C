/*
 * A072293  Smallest factorial containing exactly n 2's.
 * https://oeis.org/A072293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   smallest factorial containing exactly n 2's a(n) = min{ k! |
 *            decimal expansion of k! contains n digits equal to '2' }
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   digit_frequencies, seq_from_str, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utility headers */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/digit_frequencies.h" /* digit_frequencies */
#include "../utils/seq_mul.h"          /* seq_mul */
#include "../utils/seq_from_str.h"     /* seq_from_str */

/* ------------------------------------------------------------------ */
/* helper to convert an unsigned integer to a seq_t value            */
/* ------------------------------------------------------------------ */
static seq_t int_to_seq(uint64_t v)
{
    /* buffer large enough for 64‑bit decimal */
    char buf[32];
    snprintf(buf, sizeof(buf), "%llu", (unsigned long long)v);
    return seq_from_str(buf);
}

/* ------------------------------------------------------------------ */
/* global caches: factorials and results for each 2-count              */
/* ------------------------------------------------------------------ */
static seq_t *fact_cache = NULL;   /* fact_cache[i] = i!  (i≥0) */
static size_t   fact_cap   = 0;
static idx_t    fact_len   = 0;    /* next index to compute */

static seq_t *res_cache = NULL;   /* res_cache[t] = smallest factorial with exactly t twos */
static size_t res_cap   = 0;

/* ------------------------------------------------------------------ */
/* ensure that fact_cache can hold factorials up to index `up_to`      */
/* ------------------------------------------------------------------ */
static void ensure_factorials(idx_t up_to)
{
    if (fact_len == 0) {
        /* initialise 0! = 1 */
        fact_cache = (seq_t *)malloc(sizeof(seq_t) * 8);
        if (!fact_cache) return; /* out‑of‑memory; caller will see uninitialised result */
        fact_cache[0] = int_to_seq(1);
        fact_len = 1;
        fact_cap = 8;
    }

    while (fact_len <= up_to) {
        /* enlarge cache if necessary */
        if (fact_len == fact_cap) {
            size_t new_cap = fact_cap ? fact_cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(fact_cache, sizeof(seq_t) * new_cap);
            if (!tmp) return;
            fact_cache = tmp;
            fact_cap = new_cap;
        }

        /* compute (fact_len)! = (fact_len-1)! * fact_len */
        seq_t prev = fact_cache[fact_len - 1];
        seq_t mult = int_to_seq((uint64_t)fact_len);
        seq_t next = seq_mul(prev, mult);
        fact_cache[fact_len] = next;
        fact_len++;
    }
}

/* ------------------------------------------------------------------ */
/* ensure res_cache has space for entry n (and larger if needed)      */
/* ------------------------------------------------------------------ */
static void ensure_res_capacity(idx_t n)
{
    if ((size_t)n + 1 > res_cap) {
        size_t new_cap = res_cap ? res_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(res_cache, sizeof(seq_t) * new_cap);
        if (!tmp) return;
        memset(tmp + res_cap, 0, (new_cap - res_cap) * sizeof(seq_t));
        res_cache = tmp;
        res_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* compute factorials until the smallest k! with exactly n twos is   */
/* found; store it in res_cache[n].                                 */
/* ------------------------------------------------------------------ */
static void ensure_res_for_n(idx_t n)
{
    if (n == 0) return;          /* OEIS offset begins at 1 */
    ensure_res_capacity(n);

    if (res_cache[n] != 0) return; /* already known */

    /* start from the largest factorial we already have */
    idx_t k = fact_len; /* compute next factorial after last stored */

    while (true) {
        /* compute (k)! */
        ensure_factorials(k);

        seq_t current = fact_cache[k];
        unsigned int freq[10];
        digit_frequencies(current, freq);
        idx_t twos = freq[2];

        if ((size_t)twos < res_cap && res_cache[twos] == 0) {
            res_cache[twos] = current;
        }

        if (twos == n) break;
        k++;
    }
}

/* ------------------------------------------------------------------ */
/* public entry point                                                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; behaviour undefined for n<1 */
    ensure_res_for_n(n);
    if (res_cache[n] == 0) return 0; /* safety fall‑back */
    return res_cache[n];
}
