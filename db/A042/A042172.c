/*
 * A042172  Numerators of continued fraction convergents to sqrt(611).
 * https://oeis.org/A042172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042172: Numerators of continued fraction convergents to sqrt(611) */
/* Recurrence: p_{-1}=1, p_0=floor(sqrt(611))=24;           */
/*            p_k = a_k * p_{k-1} + p_{k-2}                     */
/*            where a_k are the terms of the continued fraction of sqrt(611).      */

#include <stddef.h>
#include <stdlib.h>

/* Constants for sqrt(611) */
static const unsigned long long D   = 611ULL;   /* radicand                     */
static const unsigned long long A0  = 24ULL;    /* floor(sqrt(611))             */

/* Cached numerators of the convergents */
static seq_t *cache  = NULL;  /* pointer to array of seq_t           */
static size_t cache_len = 0; /* number of elements stored in cache */
static size_t cache_cap = 0; /* allocated capacity of cache        */

/* Current state of the continued fraction iteration */
static unsigned long long m    = 0;             /* current m value */
static unsigned long long d    = 1;             /* current d value */
static unsigned long long a_val = A0;           /* current a value */

/* Previous two convergents needed for the recurrence */
static seq_t prev2 = (seq_t)0; /* p_{k-2} */
static seq_t prev1 = (seq_t)1; /* p_{k-1} initially 1 */

/* Number of convergents already generated (equals cache_len) */
static idx_t computed = 0; /* index of next term to compute */

/* ------------------------------------------------------------------ */
/*  seq_t a(idx_t n)   -- return the nth numerator of the convergents */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Generate terms up to n if not already cached */
    while (computed <= n) {
        /* Compute next numerator using recurrence */
        seq_t p = (seq_t)a_val * prev1 + prev2;

        /* Resize cache if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : (cache_cap << 1);
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();                 /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store new numerator */
        cache[cache_len++] = p;

        /* Update recurrence state for next iteration */
        prev2 = prev1;
        prev1 = p;

        /* Advance continued‑fraction state */
        unsigned long long m_next = d * a_val - m;
        unsigned long long d_next = (D - m_next * m_next) / d;
        unsigned long long a_next = (A0 + m_next) / d_next;

        m     = m_next;
        d     = d_next;
        a_val = a_next;

        ++computed;
    }

    return cache[n];
}
