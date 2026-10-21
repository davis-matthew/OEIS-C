/*
 * A200467  Number of 0..6 arrays x(0..n-1) of n elements with each no smaller
 *          than the sum of its four previous neighbors modulo 7.
 * https://oeis.org/A200467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n): number of arrays x(0..n-1) with elements 0..6 such that
 *  for each position i (i ≥ 4)  x[i] ≥ (x[i-1] + x[i-2] + x[i-3] + x[i-4]) mod 7.
 *  The values for n = 1,2,3 are computed directly; for n ≥ 4 a dynamic
 *  programming over the last four values is used.  The sub‑problem states
 *  are encoded as a base‑7 number from 0 to 2400.  The transition table
 *  is built once in a static local variable.  The implementation uses
 *  only integer arithmetic and works for any seq_t/idx_t types supplied
 *  by the build. */

#include <stdint.h>

static int16_t trans[2401][7];
static int trans_inited = 0;

/* Encode a,b,c,d ∈ [0,6] as a single index 0..2400 */
static inline int encode(int a, int b, int c, int d)
{
    return (((a * 7) + b) * 7 + c) * 7 + d;
}

/* Build the transition table once */
static void init_trans(void)
{
    if (trans_inited) return;
    for (int a = 0; a < 7; ++a)
    {
        for (int b = 0; b < 7; ++b)
        {
            for (int c = 0; c < 7; ++c)
            {
                for (int d = 0; d < 7; ++d)
                {
                    int idx = encode(a, b, c, d);
                    int sum = (a + b + c + d) % 7;
                    for (int e = 0; e < 7; ++e)
                    {
                        if (e >= sum)
                            trans[idx][e] = (int16_t)encode(b, c, d, e);
                        else
                            trans[idx][e] = -1;
                    }
                }
            }
        }
    }
    trans_inited = 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0;          /* offset is 1, unused below */
    if (n == 1) return (seq_t)7;          /* any of 0..6 */
    if (n == 2)                          /* non‑decreasing pairs */
    {
        seq_t cnt = 0;
        for (int x0 = 0; x0 < 7; ++x0)
            for (int x1 = x0; x1 < 7; ++x1)
                ++cnt;
        return cnt;
    }
    if (n == 3)                          /* non‑decreasing pair and
                                           second condition for x2 */
    {
        seq_t cnt = 0;
        for (int x0 = 0; x0 < 7; ++x0)
            for (int x1 = x0; x1 < 7; ++x1)
            {
                int sum12 = (x0 + x1) % 7;
                for (int x2 = sum12; x2 < 7; ++x2)
                    ++cnt;
            }
        return cnt;
    }

    /* n >= 4: dynamic programming over last four values */
    init_trans();

    seq_t cur[2401];
    seq_t nxt[2401];
    for (int i = 0; i < 2401; ++i) cur[i] = (seq_t)0;

    /* initial states: all quadruples satisfying the first four constraints */
    for (int x0 = 0; x0 < 7; ++x0)
    {
        for (int x1 = 0; x1 < 7; ++x1)
        {
            if (x1 < x0) continue;
            for (int x2 = 0; x2 < 7; ++x2)
            {
                if (x2 < (x0 + x1) % 7) continue;
                for (int x3 = 0; x3 < 7; ++x3)
                {
                    if (x3 < (x0 + x1 + x2) % 7) continue;
                    cur[encode(x0, x1, x2, x3)] = (seq_t)1;
                }
            }
        }
    }

    /* Extend the sequences one element at a time */
    for (idx_t pos = 4; pos < n; ++pos)
    {
        for (int i = 0; i < 2401; ++i) nxt[i] = (seq_t)0;
        for (int s = 0; s < 2401; ++s)
        {
            seq_t val = cur[s];
            if (val == (seq_t)0) continue;
            for (int e = 0; e < 7; ++e)
            {
                int16_t ns = trans[s][e];
                if (ns >= 0)
                    nxt[ns] += val;
            }
        }
        /* swap pointers (via copying since local arrays) */
        for (int i = 0; i < 2401; ++i)
            cur[i] = nxt[i];
    }

    /* sum all final states */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 2401; ++i)
        total += cur[i];
    return total;
}
