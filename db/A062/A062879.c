/*
 * A062879  Integers whose Zeckendorf expansion does not contain ones at even
 *          positions.
 * https://oeis.org/A062879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ bit_j(n-1) * F_{2j+1} where F_0 = 1, F_1 = 2 and
 *            F_{k+1} = F_k + F_{k-1} for k ≥ 1. The index j runs over all
 *            bits of n‑1 (least significant bit = j = 0)
 * Range:     u8 n <= 40, u16 n <= 2048, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool fibs_initialized = false;
static seq_t odd_fibs[64];          /* odd_fibs[j] = F_{2j+1} */

/* Pre‑compute odd-index Fibonacci numbers up to F_127. */
static void init_fibs(void)
{
    if (fibs_initialized) return;
    seq_t f0 = 1;      /* F_0 */
    seq_t f1 = 2;      /* F_1 */
    int idx        = 1; /* current Fibonacci index (starting at 1 = F_1) */
    odd_fibs[0]    = f1;              /* F_1 is the first odd‑index Fibonacci */
    int odd_idx    = 0;

    /* We need odd indices up to 127, so loop until idx == 127 */
    while (idx < 127)
    {
        seq_t f_next = f0 + f1;       /* F_{idx+1} = F_{idx-1} + F_{idx} */
        f0 = f1;
        f1 = f_next;
        ++idx;
        if (idx & 1)                  /* idx is odd → store */
        {
            ++odd_idx;
            odd_fibs[odd_idx] = f1;
        }
    }
    fibs_initialized = true;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 by specification */
    init_fibs();

    seq_t result = 0;
    idx_t  m      = n - 1;    /* work with n-1 */
    int    j      = 0;        /* bit position */

    while (m)
    {
        if (m & 1ULL)
            result += odd_fibs[j];
        m >>= 1;
        ++j;
    }
    return result;
}
