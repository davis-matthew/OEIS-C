/*
 * A071691  Number of ways to write n as n = x!+y!+z!+t! with
 *          0<=x<=y<=z<=t<=n.
 * https://oeis.org/A071691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{x,y,z,t | 0 ≤ x ≤ y ≤ z ≤ t ≤ n, n = x! + y! + z! + t!}
 *            . The algorithm: 1) Build all factorials ≤ n (including
 *            duplicates 0! and 1!). 2) Enumerate all non‑decreasing 4‑tuples
 *            of those indices and count the ones whose sum equals n
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static seq_t fact[64];   /* buffer for factorials; 64 is plenty for any
                            practical n in this sequence */

seq_t a(idx_t n)
{
    /* Handle tiny n directly (n ≥ 1 by the offset demand) */
    if (n < (idx_t)1)
        return (seq_t)0;

    /* 1. Build factorials up to n. */
    idx_t fcnt = 0;              /* number of factorial entries */
    fact[0] = (seq_t)1;          /* 0! = 1 */
    fcnt = 1;                    /* first entry used */

    for (idx_t i = 1; ; ++i) {
        /* Compute next factorial; stop if it would exceed n. */
        seq_t next = fact[i - 1] * (seq_t)i;   /* (i)! = (i-1)! * i */
        if (next > (seq_t)n)
            break;
        fact[i] = next;
        ++fcnt;
    }

    /* 2. Enumerate all non‑decreasing quadruples. */
    seq_t count = 0;
    for (idx_t i = 0; i < fcnt; ++i)
        for (idx_t j = i; j < fcnt; ++j)
            for (idx_t k = j; k < fcnt; ++k)
                for (idx_t l = k; l < fcnt; ++l) {
                    seq_t sum = fact[i] + fact[j] + fact[k] + fact[l];
                    if (sum == (seq_t)n)
                        ++count;
                }

    return count;
}
