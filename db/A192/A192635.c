/*
 * A192635  G.f. satisfies: A(x) = exp( Sum_{n>=1} x^n*A(x^n/(1-x^n))/n ).
 * https://oeis.org/A192635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A192635
 *
 *  This function implements OEIS A192635, defined by the generating
 *  function
 *      A(x) = exp( Σ_{n≥1} x^n*A(x^n/(1-x^n))/n )
 *
 *  The coefficients satisfy the following recurrence.  Let
 *
 *      C(m) = Σ_{t=1}^{m-1} a(t)*binom(m-2, t-1)          (C(1)=0)
 *
 *      H(m) = 1 + Σ_{d|m, d>1} d * C(d)
 *
 *  Then with a(0)=1 the sequence is obtained by
 *
 *      a(n) = (1/n) * Σ_{j=1}^{n} H(j) * a(n-j)          (n≥1)
 *
 *  This recurrence is derived from the logarithmic derivative of
 *  the exponential generating function and it yields exactly the
 *  OEIS terms.
 *
 *  The function `a` is memoised and works for calls in any order.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *a_seq = NULL;   /* a[0..]            */
static seq_t *c_seq = NULL;   /* C[0..]             */
static seq_t *h_seq = NULL;   /* H[0..]             */
static size_t capacity = 0;
static size_t maxIdx   = 0;

/* ------------------------------------------------------------------ */
/* expand the three arrays to hold indices up to 'need' (inclusive)    */
/* ------------------------------------------------------------------ */
static void grow_capacity(size_t need)
{
    if (need < capacity)
        return;
    size_t newCap = capacity ? capacity : 8;
    while (newCap <= need)
        newCap <<= 1;

    seq_t *newA = (seq_t*)malloc(newCap * sizeof(seq_t));
    seq_t *newC = (seq_t*)malloc(newCap * sizeof(seq_t));
    seq_t *newH = (seq_t*)malloc(newCap * sizeof(seq_t));
    if (!newA || !newC || !newH) {
        free(newA); free(newC); free(newH);
        exit(1);
    }

    /* copy old data */
    for (size_t i = 0; i < capacity; ++i) {
        newA[i] = a_seq[i];
        newC[i] = c_seq[i];
        newH[i] = h_seq[i];
    }
    /* zero new elements up to newCap-1 */
    for (size_t i = capacity; i < newCap; ++i) {
        newA[i] = (seq_t)0;
        newC[i] = (seq_t)0;
        newH[i] = (seq_t)0;
    }

    free(a_seq); free(c_seq); free(h_seq);
    a_seq = newA; c_seq = newC; h_seq = newH;
    capacity = newCap;
}

/* --------------------------------------------------------------- */
/* multiplicative binomial coefficient nCk (n,k are non‑negative)   */
/* --------------------------------------------------------------- */
static seq_t binom_uint(size_t n, size_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;
    seq_t res = (seq_t)1;
    for (size_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* --------------------------------------------------------------- */
/* compute a(n) – memoised                                         */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    size_t target = (size_t)n;
    if (maxIdx < target) {
        grow_capacity(target);

        /* initialise a[0] once */
        if (maxIdx == 0) {
            a_seq[0] = (seq_t)1;
        }

        /* compute new values from maxIdx+1 to target */
        for (size_t k = maxIdx + 1; k <= target; ++k) {
            /* ---- compute C(k) ---- */
            seq_t Ck = (seq_t)0;
            if (k > 1) {
                seq_t bin = (seq_t)1;          /* binom(k-2,0) */
                /* t = 1 gives a[1] * bin */
                Ck += a_seq[1] * bin;
                for (size_t t = 2; t <= k - 1; ++t) {
                    /* update bin = binom(k-2, t-1) */
                    bin = bin * (seq_t)(k - t) / (seq_t)(t - 1);
                    Ck += a_seq[t] * bin;
                }
            }
            c_seq[k] = Ck;

            /* ---- compute H(k) ---- */
            seq_t Hk = (seq_t)1;  /* contribution from divisor m=1 */
            /* iterate over divisors m of k larger than 1 */
            for (size_t d = 2; d * d <= k; ++d) {
                if (k % d == 0) {
                    Hk += (seq_t)d * c_seq[d];
                    if (d != k / d)
                        Hk += (seq_t)(k / d) * c_seq[k / d];
                }
            }
            if (k > 1)  /* k itself is a divisor >1 */
                Hk += (seq_t)k * c_seq[k];
            h_seq[k] = Hk;

            /* ---- compute a(k) ---- */
            seq_t sum = (seq_t)0;
            for (size_t j = 1; j <= k; ++j) {
                sum += h_seq[j] * a_seq[k - j];
            }
            a_seq[k] = sum / (seq_t)k;
        }

        maxIdx = target;
    }

    return a_seq[target];
}
