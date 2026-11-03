/*
 * A227457  E.g.f. equals the series reversion of x - x*log(1+x).
 * https://oeis.org/A227457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n-1} k! * s(n-1,k) * binom(n+k-1,n-1) where
 *            s(n,k) are the (signed) Stirling numbers of the first kind.
 *            Since s(n,k) = (-1)^{n-k} * |s(n,k)|, we use the unsigned
 *            Stirling numbers c(n,k)=|s(n,k)| and accumulate with alternating
 *            signs
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1, unused case */

    idx_t m = n - 1;                      /* we need Stirling numbers for (n-1,k) */

    /* Allocate array for unsigned Stirling numbers of the first kind,
       c[k] = |s(m,k)| for current m.  */
    seq_t *c = mem_new(seq_t, (m + 1));
    if (!c) return 0; /* fallback on failure */

    /* Initialize row 0: c[0] = 1, others 0.  */
    for (idx_t i = 0; i <= m; ++i) c[i] = 0;
    c[0] = 1;

    /* Build row m using recurrence c(n,k)=c(n-1,k-1)+(n-1)*c(n-1,k).  */
    for (idx_t i = 1; i <= m; ++i) {
        /* update from high k to low to use previous row values */
        for (idx_t k = i; k >= 1; --k) {
            seq_t term = mul_small(c[k], (unsigned int)(i - 1));  /* (i-1)*c(n-1,k) */
            c[k] = term + c[k - 1];                            /* + c(n-1,k-1) */
        }
        c[0] = 0;  /* c(n,0)=0 for n>0 */
    }

    /* Accumulate terms with alternating signs:  (-1)^{m-k}.  */
    seq_t pos = 0;   /* sum of terms with even (m-k)  */
    seq_t neg = 0;   /* sum of terms with odd  (m-k) */

    seq_t fact = 1;          /* k!  */
    seq_t binom_val = 1;     /* binomial(n+k-1, n-1)  for current k */

    for (idx_t k = 0; k <= m; ++k) {
        /* term = fact * c[k] * binom_val  (all positive) */
        seq_t term = fact;
        term *= c[k];
        term *= binom_val;

        if (((m - k) & 1) == 0)   /* even -> positive contribution */
            pos += term;
        else
            neg += term;

        /* Prepare for next k if any */
        if (k < m) {
            fact = mul_small(fact, (unsigned int)(k + 1));           /* (k+1)!    */
            /* update binom: binom_val = binom_val * (n+k) / (k+1) */
            binom_val = binom_val * (idx_t)(n + k);  /* use normal * because seq_t can handle */
            binom_val /= (idx_t)(k + 1);
        }
    }

    mem_free(c);

    /* Result = pos - neg  (pos >= neg for all n) */
    return pos - neg;
}
