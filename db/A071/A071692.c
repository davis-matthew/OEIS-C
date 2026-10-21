/*
 * A071692  Number of ways to write n as n = x! + y! + z! where 0<=x<=y<=z<=n.
 * https://oeis.org/A071692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2775, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of triples (x,y,z) with 0 <= x <= y <= z <= n
 * such that x! + y! + z! = n.
 * The function computes this count by enumerating all factorial indices
 * <= n and testing every non‑decreasing triple.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Convert n to seq_t for arithmetic with arbitrary precision types. */
    seq_t nseq = (seq_t)n;

    /* Pre‑compute factorials f[i] = i! for all i with i! <= n. */
    seq_t fac[64];
    int k = 0;                       /* number of stored factorials */
    seq_t fact = 1;                  /* current factorial value */
    int i = 0;
    while (1) {
        if (fact > nseq) break;      /* stop when factorial exceeds n */
        if (k >= 64) break;          /* safety limit */
        fac[k++] = fact;             /* store i! */

        /* compute next factorial: (i+1)! */
        fact = fact * (seq_t)(i + 1);
        i++;
    }

    /* Count all triples (i,j,l) with i <= j <= l and fac[i]+fac[j]+fac[l]==n. */
    seq_t count = 0;
    for (int i = 0; i < k; ++i) {
        for (int j = i; j < k; ++j) {
            for (int l = j; l < k; ++l) {
                /* Avoid intermediate overflow by using seq_t arithmetic. */
                seq_t sum = fac[i] + fac[j] + fac[l];
                if (sum == nseq) count++;
            }
        }
    }
    return count;
}
