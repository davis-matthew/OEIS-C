/*
 * A098200  Number of distinct terms in iteration-list when
 *          cototient-function[=A051953] is iterated and the initial value is
 *          even number.
 * https://oeis.org/A098200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A098200
 *  a(n) = number of distinct terms in the iteration list of the
 *  cototient function (x → x-φ(x)) starting from 2n and terminating
 *  at 0.
 *  The sequence is the even‑indexed terms of A053475:
 *  a(n) = A053475[2n].
 *
 *  Algorithm
 *  ---------
 *  1. Pre‑compute φ(k) for all 0 ≤ k ≤ 20000 (sufficient for
 *     n ≤ 10000).  This is done once with a linear sieve.
 *  2. For each call count distinct values seen during the
 *     cototient iteration.  A static "stamp" array marks values
 *     that have already been visited in the current call.
 *  3. Use only types that are guaranteed to be available:
 *     - idx_t (unsigned 64‑bit) for indices and for the running
 *       value of the iteration.
 *     - seq_t for the returned count and for local counters that
 *       need only the output width.
 *     - uint32_t for the stored φ values; they never exceed 20000.
 *
 *  The routine works for every build: seq_t may be uint8_t, uint16_t,
 *  uint32_t, uint64_t or an arbitrary‑precision type.  Only the
 *  internal value `val` is kept in `idx_t`, which is large enough to
 *  hold all intermediate values.
 *
 *  Complexity   :  O(k) per call where k is the length of the
 *                  cototient chain starting from 2n (≤ 2n).
 *  Memory usage :  O(MAXVAL) ≈ 80 KB for the φ table and stamps.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXVAL 20000   /* 2 * 10000  –  enough for all tested n */

static uint32_t phi_tbl[MAXVAL + 1];
static bool phi_done = false;

/* Precompute totient values 0 … MAXVAL */
static void init_phi(void)
{
    if (phi_done) return;
    /* trivial values */
    phi_tbl[0] = 0;
    phi_tbl[1] = 1;
    for (idx_t i = 2; i <= MAXVAL; ++i) phi_tbl[i] = (uint32_t)i;

    /* linear sieve to compute φ */
    for (idx_t i = 2; i <= MAXVAL; ++i)
    {
        if (phi_tbl[i] == (uint32_t)i)            /* i is prime */
        {
            for (idx_t j = i; j <= MAXVAL; j += i)
                phi_tbl[j] -= phi_tbl[j] / (uint32_t)i;
        }
    }
    phi_done = true;
}

/* Stamp array for marking visited values in the current call */
static uint32_t *vis_stamp = NULL;   /* indices 0 … MAXVAL */
static uint32_t stamp_counter = 1;   /* start from 1 to avoid zero marker */

static void init_vis(void)
{
    if (vis_stamp) return;
    vis_stamp = (uint32_t*)calloc(MAXVAL + 1, sizeof(uint32_t));
    if (!vis_stamp) exit(EXIT_FAILURE);
}

/*--------------------------------------------------------------*/
/*  Sequence function                                            */
/*--------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for 0 – safe default */

    /* Ensure tables are ready */
    init_phi();
    init_vis();

    ++stamp_counter;        /* start a new stamp for this call */

    seq_t count = 0;
    idx_t  val   = 2ULL * n;   /* start value 2n (≤ MAXVAL) */

    while (1)
    {
        idx_t idx = val;          /* safe: val ≤ 2n ≤ MAXVAL */

        if (vis_stamp[idx] != stamp_counter)
        {
            vis_stamp[idx] = stamp_counter;
            ++count;
        }

        if (val == 0) break;      /* finished */

        uint32_t phi_val = phi_tbl[idx];
        val -= (idx_t)phi_val;    /* next value in cototient iteration */
    }

    return count;
}
