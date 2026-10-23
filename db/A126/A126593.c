/*
 * A126593  Numbers that belong to a cycle under the map k = Sum d_i 10^i ->
 *          f(k) = Sum d_i 2^i.
 * https://oeis.org/A126593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   _f
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126593: numbers that belong to a cycle under
   f(k) = Σ digit 2^digit  (digit = decimal digit of k).
   The function returns the n‑th smallest such number, with a(1)=5. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_f.h"

#define LIM 5000  /* covers all cycle elements, see OEIS comments */

static char state[LIM + 1] = {0};    /* 0 = unvisited, 1 = visiting, 2 = done */
static char is_cycle[LIM + 1] = {0}; /* 1 if the number is in a cycle   */
static seq_t cycle_vals[LIM + 1] = {0};
static idx_t cycle_count = 0;
static bool cycles_built = false;

/* -------------------------------------------------------------
   Build the list of all cycle elements (≤ LIM)
   ------------------------------------------------------------- */
static void _build_cycles(void)
{
    int stk[LIM];
    /* Identify all cycles */
    for (int start = 1; start <= LIM; ++start) {
        if (state[start] != 0) continue;

        int cur = start;
        int sp = 0; /* stack pointer */

        while (1) {
            if (cur > LIM) break;               /* outside range – nothing to do */
            if (state[cur] == 0) {
                state[cur] = 1;
                stk[sp++] = cur;
                cur = (int)_f((seq_t)cur);
                continue;
            }
            if (state[cur] == 1) {              /* cycle found */
                int pos = 0;
                while (stk[pos] != cur) pos++;
                for (int j = pos; j < sp; ++j)
                    is_cycle[stk[j]] = 1;
                break;
            }
            /* state[cur] == 2 : reached processed node */
            break;
        }
        /* Mark all nodes on the stack as processed */
        for (int j = 0; j < sp; ++j)
            state[stk[j]] = 2;
    }

    /* Collect cycle elements and sort them */
    cycle_count = 0;
    for (int i = 1; i <= LIM; ++i)
        if (is_cycle[i])
            cycle_vals[cycle_count++] = (seq_t)i;

    /* Simple insertion sort (cycle_count ≤ LIM) */
    for (idx_t i = 1; i < cycle_count; ++i) {
        seq_t key = cycle_vals[i];
        idx_t j = i;
        while (j > 0 && cycle_vals[j - 1] > key) {
            cycle_vals[j] = cycle_vals[j - 1];
            --j;
        }
        cycle_vals[j] = key;
    }

    cycles_built = true;
}

/* -------------------------------------------------------------
   Public sequence generator
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!cycles_built)
        _build_cycles();

    if (n == 0 || n > cycle_count)
        return (seq_t)0;
    return cycle_vals[n - 1];
}
