/*
 * A088501  Expansion of e.g.f. 1/(1-2*log(1+x)).
 * https://oeis.org/A088501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*∑_{k=1..n}(-1)^(k-1)*(k-1)!*binomial(n,k)*a(n-k)
 *            Initial values: a(0)=1, a(1)=2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *a_seq = NULL;
static size_t a_cap = 0;
static seq_t *fact  = NULL;
static size_t fact_cap = 0;

static void ensure_cap(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (a_cap < needed) {
        if (a_seq == NULL)
            a_seq = seq_alloc(needed);
        else
            a_seq = seq_realloc(a_seq, a_cap, needed);
        a_cap = needed;
    }
    if (fact_cap < needed) {
        if (fact == NULL)
            fact = seq_alloc(needed);
        else
            fact = seq_realloc(fact, fact_cap, needed);
        fact_cap = needed;
    }
}

seq_t a(idx_t n)
{
    /* ensure arrays are large enough */
    const size_t old_a_cap   = a_cap;
    const size_t old_fact_cap = fact_cap;
    ensure_cap(n);

    /* compute new entries only */
    for (size_t i = old_a_cap; i < a_cap; ++i) {
        if (i == 0) {
            a_seq[0] = 1;
            fact[0]  = 1;
            continue;
        }

        /* factorial i = (i-1)! * i */
        fact[i] = fact[i - 1] * (seq_t)i;

        /* recurrence */
        seq_t pos = 0, neg = 0;
        seq_t binom = 1; /* binomial(i,0) */
        for (idx_t k = 1; k <= (idx_t)i; ++k) {
            binom = binom * (seq_t)(i - k + 1) / (seq_t)k;     /* C(i,k) */
            seq_t term = fact[(size_t)k - 1];                  /* (k-1)!   */
            term = term * binom;                               /* *C(i,k)  */
            term = term * a_seq[i - (size_t)k];                /* *a(n-k)  */
            if (k & 1)
                pos += term;
            else
                neg += term;
        }
        a_seq[i] = (pos - neg) * (seq_t)2;
    }
    return a_seq[(size_t)n];
}
