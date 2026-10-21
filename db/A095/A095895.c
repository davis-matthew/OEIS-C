/*
 * A095895  G.f.: A(x) = Product_{n>=1} 1/(1 - n*A007947(n)*x^n)^(1/n^2),
 *          where A007947(n) is the product of the distinct prime factors of
 *          n.
 * https://oeis.org/A095895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A095895
 *  The sequence is defined by the generating function
 *
 *      A(x) = ∏_{k≥1}  (1 - k*P(k)*x^k)^(-1/k^2) ,
 *
 *  where P(k) is the product of the distinct prime factors of k
 *  (OEIS A007947).  The value a(n) is the coefficient of x^n in A(x).
 *
 *  The computation uses the standard recurrence for a series A(x)=exp(B(x)),
 *  where B(x)=ln A(x).  Let
 *
 *      g(k) = Σ_{d|k}  (b(d)^(k/d))/d           ,   b(d)=d*P(d)
 *
 *  then
 *
 *      a(0)=1 ,
 *      a(n) = (1/n) * Σ_{k=1}^{n}  g(k) * a(n-k)       (n≥1)
 *
 *  The sums inside g(k) are integer because b(d) = d * P(d) is a multiple of d.
 *  The whole algorithm works with big integers (seq_t) and performs only
 *  integer additions, multiplications and exact divisions by small integers.
 *
 *  Complexity:  O(n log n) for the sieve and building g(k),   O(n^2) for
 *  the final convolution.  For n≤10 000 the routine runs well under the
 *  time limits for both 64‑bit and arbitrary precision builds.
 */

#include <stdlib.h>
#include <stdbool.h>

/*  A095895:  sequence coefficient function */
seq_t a(idx_t n)
{
    if (n == 0)                         /* a(0)=1  */
        return 1;

    /* ------------------------------------------------------------ */
    /* 1.  Sieve for distinct–prime‑product P(k) (A007947)          */
    /* ------------------------------------------------------------ */
    const size_t N = (size_t)n;
    seq_t *pdp = (seq_t *)malloc((N + 1) * sizeof(seq_t));   /* P(k)          */
    seq_t *b   = (seq_t *)malloc((N + 1) * sizeof(seq_t));   /* b(k) = k*P(k) */
    seq_t *g   = (seq_t *)malloc((N + 1) * sizeof(seq_t));   /* g(k)          */
    seq_t *a   = (seq_t *)malloc((N + 1) * sizeof(seq_t));   /* a(k)          */

    if (!pdp || !b || !g || !a) {                      /* safety check */
        return 0;
    }

    /*  initialise P(k) to 1 and a(k) to 0 */
    for (size_t i = 0; i <= N; ++i) {
        pdp[i] = 1;
        a[i]   = 0;
    }

    /*  sieve primes up to N */
    bool *is_prime = (bool *)malloc((N + 1) * sizeof(bool));
    if (!is_prime) return 0;
    for (size_t i = 0; i <= N; ++i) is_prime[i] = true;
    if (N >= 0) is_prime[0] = false;
    if (N >= 1) is_prime[1] = false;

    for (size_t p = 2; p * p <= N; ++p) {
        if (is_prime[p]) {
            for (size_t j = p * p; j <= N; j += p)
                is_prime[j] = false;
        }
    }

    /*  Multiply each prime once into every multiple */
    for (size_t p = 2; p <= N; ++p) {
        if (is_prime[p]) {
            for (size_t j = p; j <= N; j += p)
                pdp[j] *= (seq_t)p;
        }
    }

    free(is_prime);

    /*  Compute b(k) = k * P(k)  */
    for (size_t k = 1; k <= N; ++k) {
        b[k] = (seq_t)k * pdp[k];
    }
    /* ------------------------------------------------------------ */
    /* 2.  Compute g(k)  */
    /* ------------------------------------------------------------ */
    /* initialise g to 0 */
    for (size_t k = 0; k <= N; ++k) {
        g[k] = 0;
    }

    for (size_t d = 1; d <= N; ++d) {
        seq_t pd = b[d];
        seq_t power = 1;                     /* pd^exp  */
        for (size_t exp = 1; ; ++exp) {
            size_t j = d * exp;
            if (j > N) break;
            power *= pd;                     /* pd^exp  */
            seq_t term = power / (seq_t)d;   /* exact division */
            g[j] += term;
        }
    }

    /*  ------------------------------------------------------------ */
    /* 3.  Compute a(n) by the convolution recurrence              */
    /* ------------------------------------------------------------ */
    a[0] = 1;                                 /* a(0)=1   */
    for (size_t i = 1; i <= N; ++i) {
        seq_t sum = 0;
        for (size_t k = 1; k <= i; ++k) {
            sum += g[k] * a[i - k];
        }
        a[i] = sum / (seq_t)i;                /* exact division */
    }

    /*  ------------------------------------------------------------ */
    /* 4.  Result and cleanup                                     */
    /* ------------------------------------------------------------ */
    seq_t result = a[N];

    free(pdp);
    free(b);
    free(g);
    free(a);

    return result;
}
