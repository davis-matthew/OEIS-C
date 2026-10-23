/*
 * A040730  Continued fraction for sqrt(758).
 * https://oeis.org/A040730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction partial quotients of sqrt(758) a(0) =
 *            floor(sqrt(758)) = 27 For n>0, a(n) is the n‑th term in the
 *            periodic part of the continued fraction
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* static state for the continued fraction cycle */
static seq_t *cycle = NULL;        /* array of period terms (a1 … aL) */
static size_t cycle_len = 0;      /* number of terms in the period */
static size_t cycle_cap = 0;      /* allocated capacity of cycle */
static bool initialized = false;  /* true after init_cycle() */
static seq_t a0 = 0;              /* integer part of sqrt(758) */

static void init_cycle(void)
{
    const int D = 758;
    int a0_int = 0;

    /* compute floor(sqrt(D)) without floating point */
    while ((a0_int + 1) * (a0_int + 1) <= D)
        ++a0_int;

    a0 = (seq_t)a0_int;

    /* initial capacity guess – the period is short for this D */
    cycle_cap = 10;
    cycle = (seq_t *)malloc(cycle_cap * sizeof(seq_t));
    cycle_len = 0;

    int m = 0;
    int d = 1;
    int a = a0_int;

    while (1) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0_int + m) / d;

        /* ensure capacity */
        if (cycle_len == cycle_cap) {
            cycle_cap <<= 1;
            cycle = (seq_t *)realloc(cycle, cycle_cap * sizeof(seq_t));
        }
        cycle[cycle_len++] = (seq_t)a;

        /* period ends when a == 2 * a0_int */
        if (a == 2 * a0_int)
            break;
    }

    initialized = true;
}

/* Requested sequence function */
seq_t a(idx_t n)
{
    if (!initialized)
        init_cycle();

    if (n == 0)
        return a0;

    /* (n-1) modulo cycle_len using idx_t arithmetic */
    idx_t idx = (n - 1) % (idx_t)cycle_len;
    return cycle[(size_t)idx];
}
