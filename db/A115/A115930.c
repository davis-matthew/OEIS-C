/*
 * A115930  Numbers k such that k and 5*k, taken together, are zeroless
 *          pandigital.
 * https://oeis.org/A115930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: k such that the digits of k and 5·k together use each of the digits 1–9 exactly once (no zeros). */
/* The values are computed by brute force over all 4‑digit numbers 2000–9999.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t *solutions = NULL;
static size_t solution_count = 0;

/* Generate all solutions once. */
static void
init_solutions(void)
{
    if (solutions != NULL)
        return;

    /* Allocate initial space for solutions. */
    size_t cap = 1024;
    solutions = (seq_t *)calloc(cap, sizeof(seq_t));
    if (solutions == NULL)
        return; /* allocation failure – results remain unavailable */

    for (idx_t k = 2000; k <= 9999; ++k)
    {
        bool used[10] = {0};
        seq_t num = (seq_t)k;
        int digits_k = 0;
        bool ok = true;

        /* Process digits of k. */
        seq_t t = num;
        while (t > (seq_t)0)
        {
            unsigned int d = (unsigned int)(t % (seq_t)10);
            if (d == 0 || used[d]) { ok = false; break; }
            used[d] = true;
            digits_k++;
            t /= (seq_t)10;
        }
        if (!ok || digits_k != 4)
            continue;

        /* Compute 5·k and ensure it has 5 digits. */
        seq_t m = num * (seq_t)5;
        if (m < (seq_t)10000 || m > (seq_t)99999)
            continue;

        /* Process digits of 5·k. */
        t = m;
        int digits_used = digits_k;
        while (t > (seq_t)0)
        {
            unsigned int d = (unsigned int)(t % (seq_t)10);
            if (d == 0 || used[d]) { ok = false; break; }
            used[d] = true;
            digits_used++;
            t /= (seq_t)10;
        }
        if (!ok || digits_used != 9)
            continue;

        /* Store a valid solution. */
        if (solution_count == cap)
        {
            cap *= 2;
            seq_t *new_ptr = (seq_t *)realloc(solutions, cap * sizeof(seq_t));
            if (new_ptr == NULL)
                return; /* allocation failure – cannot store more solutions */
            solutions = new_ptr;
        }
        solutions[solution_count++] = num;
    }
}

seq_t
a(idx_t n)
{
    init_solutions();
    if (n == 0 || n > solution_count)
        return (seq_t)0;      /* out of bounds – return 0 as safe sentinel */
    return solutions[n - 1];
}
