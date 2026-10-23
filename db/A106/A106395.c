/*
 * A106395  a(n) = sum of terms of n-th row of A106394.
 * https://oeis.org/A106395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 9, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute a(n) = sum of denominators of the greedy Egyptian
    fraction representation of the harmonic number H_n.
    The greedy algorithm is applied to the reduced fraction
    H_n = p/q.  After extracting the integer part, each unit
    fraction 1/d contributes d to the sum.  The computation
    uses only integer arithmetic on the supplied seq_t type.   */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd_seq.h"

static seq_t *a_arr = NULL;      /* cached a(i) values */
static idx_t capacity = 0;       /* highest n computed so far */
static seq_t last_p = 0;         /* p/q for the current n */
static seq_t last_q = 1;

/* Ensure that a_arr[1..n] are computed and cached. */
static void ensure(idx_t n)
{
    if (n <= capacity) return;

    /* allocate space for up to index n (inclusive) */
    size_t new_cap = (size_t)n + 1;        /* n+1 to include a[n] */
    seq_t *tmp = (seq_t *)realloc(a_arr, sizeof(seq_t) * new_cap);
    if (!tmp) abort();                    /* out of memory */
    a_arr = tmp;

    for (idx_t i = capacity + 1; i <= n; ++i) {
        /* update H_i = H_{i-1} + 1/i   as p/q */
        seq_t new_p = last_p * (seq_t)i + last_q;
        seq_t new_q = last_q * (seq_t)i;
        seq_t g = gcd_seq(new_p, new_q);
        new_p /= g;
        new_q /= g;
        last_p = new_p;
        last_q = new_q;

        /* greedy Egyptian fraction decomposition of H_i */
        seq_t sum = 0;
        seq_t rp = last_p;
        seq_t rq = last_q;

        /* integer part contributes 1/1 terms */
        seq_t int_part = rp / rq;
        sum += int_part;
        rp = rp % rq;

        while (rp != 0) {                 /* unit fraction part */
            seq_t d = (rq + rp - 1) / rp; /* ceil(rq/rp) */
            sum += d;
            seq_t new_rp = rp * d - rq;
            seq_t new_rq = rq * d;
            seq_t g2 = gcd_seq(new_rp, new_rq);
            rp = new_rp / g2;
            rq = new_rq / g2;
        }

        a_arr[i] = sum;
    }
    capacity = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure(n);
    return a_arr[n];
}
