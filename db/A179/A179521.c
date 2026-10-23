/*
 * A179521  The number of (nontrivial) cycles in the symmetric group S_2n that
 *          involve equally many elements in {1,...,n} and {n+1,...,2n}.
 * https://oeis.org/A179521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (2*k-1)! * binomial(n,k)^2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   allocate_seq_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses cached factorials up to (2*n-1)! and incremental binomial calculation. */
#include <stddef.h>
#include "../utils/allocate_seq_array.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    static seq_t *fac = NULL;
    static size_t fac_cap = 0;           /* number of factorial entries stored */
    idx_t need = 2 * n;                  /* need entries 0 .. 2*n-1 */

    /* Allocate and compute factorials up to required size */
    if (fac == NULL) {
        fac = allocate_seq_array((size_t)need);
        fac[0] = (seq_t)1;
        for (idx_t i = 1; i < need; ++i)
            fac[i] = fac[i - 1] * (seq_t)i;
        fac_cap = (size_t)need;
    } else if ((size_t)need > fac_cap) {
        seq_t *tmp = seq_realloc(fac, fac_cap, (size_t)need);
        fac = tmp;
        for (idx_t i = (idx_t)fac_cap; i < need; ++i)
            fac[i] = fac[i - 1] * (seq_t)i;
        fac_cap = (size_t)need;
    }

    seq_t sum = (seq_t)0;

    /* Binomial coefficient C(n,1) = n */
    seq_t binom_k = (seq_t)n;
    sum += fac[1] * binom_k * binom_k;      /* k = 1, (2*1-1)! = 1! */

    /* For k = 2 .. n use recurrence: C(n,k) = C(n,k-1)*(n-k+1)/k */
    for (idx_t k = 2; k <= n; ++k) {
        binom_k = binom_k * (seq_t)(n - k + 1) / (seq_t)k;
        sum += fac[2 * k - 1] * binom_k * binom_k;
    }

    return sum;
}
