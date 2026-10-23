/*
 * A042020  Numerators of continued fraction convergents to sqrt(534).
 * https://oeis.org/A042020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 11, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0)=23, a(1)=208;
    For n≥2 let cₙ be the (n‑th) coefficient of the continued‑fraction
    expansion of √534 (c₀=23, c₁=9, c₂=4, …).  Then
    a(n)=cₙ·a(n‑1)+a(n‑2).  */

#include <stdlib.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* ---------- static state – persisted between calls ---------- */
    static idx_t        cur_m = 0;           /* current m in CF */
    static idx_t        cur_d = 1;           /* current d in CF */
    static idx_t        cur_a = 23;          /* current a in CF ("cₙ") */
    static idx_t        cur_index = 0;       /* next index to compute */
    static seq_t        prevprev = 0;        /* a(k‑2) */
    static seq_t        prev     = 1;        /* a(k‑1) */
    static seq_t        *numer = NULL;       /* array of numerators */
    static size_t       alloc_cap = 0;       /* allocated capacity */
    static size_t       cache_sz = 0;        /* number of terms stored */

    const idx_t D = 534;      /* constant in the quadratic */
    const idx_t a0 = 23;      /* floor(sqrt(D)) */

    /* initialise if first call */
    if (!n && cur_index == 0 && !numer)
        ;  /* nothing to do – all static vars are zero/initialised at file scope */

    /* compute terms until n is reached */
    while (cur_index <= n)
    {
        /* 1. compute next numerator  p = cₙ·prev + prevprev  */
        seq_t p = (seq_t)cur_a * prev + prevprev;

        /* 2. store it (ensure capacity) */
        if (cache_sz >= alloc_cap)
        {
            size_t new_cap = (alloc_cap == 0) ? 64 : alloc_cap * 2;
            seq_t *tmp = seq_realloc(numer, alloc_cap, new_cap);
            if (!tmp) exit(1);          /* out of memory – abort */
            numer = tmp;
            alloc_cap = new_cap;
        }
        numer[cache_sz++] = p;

        /* 3. advance the continued‑fraction state to get next c_{n+1} */
        idx_t m_next = cur_d * cur_a - cur_m;
        idx_t d_next = (D - m_next * m_next) / cur_d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* 4. shift recurrence variables for next step */
        prevprev = prev;
        prev = p;
        cur_index++;

        /* 5. set up state for upcoming iteration */
        cur_m = m_next;
        cur_d = d_next;
        cur_a = a_next;
    }

    return numer[n];
}
