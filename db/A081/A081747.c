/*
 * A081747  a(n) = A081746(n) - n.
 * https://oeis.org/A081747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A081746(n) - n; periodic with period 648, so a(n) =
 *            a(((n-1) mod 648)+1). Table of one period computed from the
 *            A081746 recurrence: A081746(1)=1, A081746(k) = A081746(k-1)-3 if
 *            k already occurred as a term, else A081746(k-1)+5
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A081747_PERIOD 648
#define A081747_MAXV 8192

static uint64_t A081747_tab[A081747_PERIOD + 1];
static bool A081747_ready = false;

static void A081747_init(void)
{
    static bool seen[A081747_MAXV];
    uint64_t prev = 1;
    for (int i = 0; i < A081747_MAXV; ++i) seen[i] = false;
    seen[1] = true;
    A081747_tab[1] = 0; /* 1 - 1 */
    for (uint64_t k = 2; k <= A081747_PERIOD; ++k) {
        bool in_seq = (k < A081747_MAXV) && seen[k];
        uint64_t v = in_seq ? prev - 3 : prev + 5;
        if (v < A081747_MAXV) seen[v] = true;
        A081747_tab[k] = v - k;
        prev = v;
    }
    A081747_ready = true;
}

seq_t a(idx_t n)
{
    if (!A081747_ready) A081747_init();
    if (n == 0) return 0;
    idx_t m = (n - 1) % A081747_PERIOD + 1;
    return (seq_t)A081747_tab[m];
}
