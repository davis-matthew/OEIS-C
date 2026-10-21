/*
 * A088517  First differences of Golomb's sequence.
 * https://oeis.org/A088517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001462(n+1) - A001462(n) where A001462 is Golomb's
 *            self-describing sequence: G(1)=1, G(n)=1+G(n-G(G(n-1))) for n>1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  We lazily build the Golomb array up to the needed index.  The array
    is 1–based; index 0 is unused.  The implementation supports both
    C11 and C++17.  For C++ the lib may provide an arbitrary‑precision
    seq_t;  we use a plain realloc for all builds, casting the result
    to (seq_t*) so the code compiles in both languages.   */

#include <stdlib.h>
#include <stdint.h>

static seq_t *Golomb = NULL;        /* 1‑based array of G values          */
static idx_t   Golomb_len = 0;     /* highest index that has been computed */
static idx_t   Golomb_cap = 0;     /* allocated capacity for Golomb       */

/* Ensure that Golomb[] has capacity for at least 'need' indices (1‑based). */
static void ensure_golomb_capacity(idx_t need)
{
    if (Golomb_cap >= need) return;
    /* Grow capacity exponentially to keep O(n) overall time. */
    idx_t new_cap = (Golomb_cap == 0) ? 16 : Golomb_cap * 2;
    while (new_cap < need) new_cap *= 2;
    /* realloc returns void* in C; cast explicitly for both C and C++. */
    Golomb = (seq_t *)realloc(Golomb, (new_cap + 1) * sizeof(seq_t));
    Golomb_cap = new_cap;
}

/* Compute G[i] for all i from current Golomb_len+1 up to 'max' (inclusive). */
static void compute_golomb_to(idx_t max)
{
    if (Golomb_len == 0)
    {
        /* Initial seed for Golomb's sequence. */
        ensure_golomb_capacity(16);
        Golomb[1] = (seq_t)1;      /* G(1) = 1 */
        Golomb_len = 1;
    }
    ensure_golomb_capacity(max);
    for (idx_t i = Golomb_len + 1; i <= max; ++i)
    {
        /* recurrence: G(i) = 1 + G(i - G(G(i-1))) */
        idx_t g_prev   = (idx_t)Golomb[i - 1];          /* G(i-1)  */
        idx_t g_g_prev = (idx_t)Golomb[g_prev];        /* G(G(i-1)) */
        idx_t sub_idx  = i - g_g_prev;                 /* i - G(G(i-1)) */
        Golomb[i] = (seq_t)1 + Golomb[sub_idx];
    }
    Golomb_len = max;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* a(1) = G(2) - G(1);  need G(n+1) and G(n).  Guard index 0. */
    if (n == 0) return (seq_t)0;  /* undefined for 0 but return 0 for safety */
    compute_golomb_to(n + 1);
    return Golomb[(idx_t)(n + 1)] - Golomb[(idx_t)n];
}
