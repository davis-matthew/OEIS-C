/*
 * A226638  Product of Pell and Lucas numbers.
 * https://oeis.org/A226638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Pell(n) * Lucas(n) Pell: P(0)=0, P(1)=1,
 *            P(n)=2*P(n-1)+P(n-2) Lucas: L(0)=2, L(1)=1, L(n)=L(n-1)+L(n-2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   ensure_capacity, mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#include "../utils/ensure_capacity.h"
#include "../utils/mul_small.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    static seq_t *P = NULL;
    static seq_t *L = NULL;
    static size_t p_cap = 0;
    static size_t l_cap = 0;
    static size_t computed = 0;

    /* initialize the first two terms once */
    if (computed == 0) {
        ensure_capacity(2, &p_cap, (void **)&P, sizeof(seq_t));
        ensure_capacity(2, &l_cap, (void **)&L, sizeof(seq_t));
        P[0] = (seq_t)0;
        P[1] = (seq_t)1;
        L[0] = (seq_t)2;
        L[1] = (seq_t)1;
        computed = 2;
    }

    /* extend to n if necessary */
    if ((size_t)n >= computed) {
        size_t new_cap = (size_t)n + 1;
        ensure_capacity(new_cap, &p_cap, (void **)&P, sizeof(seq_t));
        ensure_capacity(new_cap, &l_cap, (void **)&L, sizeof(seq_t));

        for (size_t i = computed; i <= (size_t)n; ++i) {
            /* Pell recurrence: P[i] = 2*P[i-1] + P[i-2] */
            seq_t two_pi_minus1 = mul_small(P[i-1], 2);
            P[i] = two_pi_minus1 + P[i-2];

            /* Lucas recurrence: L[i] = L[i-1] + L[i-2] */
            L[i] = L[i-1] + L[i-2];
        }
        computed = new_cap;
    }

    return seq_mul(P[(size_t)n], L[(size_t)n]);
}
