/*
 * A116545  Sum of the largest Fibonacci exponent of prime factorizations of
 *          k, k=1..n.
 * https://oeis.org/A116545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=1..n} f(k) f(k) = largest Fibonacci number that
 *            occurs among the exponents in the prime factorisation of k; if
 *            no exponent is a Fibonacci number, f(k) = 1
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>

/* Global static helpers --------------------------------------------------- */

/* Smallest‑prime‑factor table: spf[i] = smallest prime that divides i (i ≥ 2) */
static uint32_t *spf = NULL;
static size_t   spf_len = 0;

/* Build sieved smallest‑prime‑factor table up to n (inclusive). */
static void build_spf(size_t n)
{
    if (spf_len >= n + 1) return;
    free(spf);
    spf_len = n + 1;
    spf = (uint32_t *)malloc(spf_len * sizeof(uint32_t));
    if (!spf) abort();

    for (size_t i = 0; i < spf_len; ++i) spf[i] = 0;
    for (size_t i = 2; i < spf_len; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = (uint32_t)i;
            if (i <= spf_len / i) {
                for (size_t j = i * i; j < spf_len; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
}

/* Fibonacci‑membership table – up to exponent 63 is safe for all n ≤ 10⁶. */
static unsigned char fib_is[64] = {0};

/* Initialise fib_is once. */
static void init_fib_table(void)
{
    if (fib_is[0]) return;           /* already initialised */
    unsigned a = 1, b = 1, i = 1;
    fib_is[1] = 1;                   /* 1 is a Fibonacci number */
    while (a < 64) {
        fib_is[a] = 1;
        unsigned c = a + b;
        a = b;
        b = c;
        ++i;
    }
}

/* Cumulative sums a(k) for k = 0..last_computed (index 0 unused). */
static seq_t *cum_sum = NULL;
static size_t cum_last = 0;          /* last index that has been computed */

/* ------------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* defined only for n ≥ 1 */

    /* Ensure helper structures are ready */
    size_t N = (size_t)n;
    build_spf(N);
    init_fib_table();

    /* Allocate or expand cumulative‑sum array */
    if (!cum_sum) {
        cum_sum = (seq_t *)calloc(N + 1, sizeof(seq_t));
        if (!cum_sum) abort();
        cum_sum[0] = (seq_t)0;
        cum_last   = 0;
    } else if (N > cum_last) {
        seq_t *tmp = (seq_t *)realloc(cum_sum, (N + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cum_sum = tmp;
    }

    /* Compute missing values */
    for (size_t k = cum_last + 1; k <= N; ++k) {
        seq_t add;
        if (k == 1) {
            add = (seq_t)1;
        } else {
            /* Find largest Fibonacci exponent in factorisation of k */
            unsigned maxFib = 1;        /* 1 is always available */
            uint64_t t = k;
            while (t > 1) {
                uint32_t p = spf[(size_t)t];
                unsigned exp = 0;
                while (t % p == 0) {
                    t /= p;
                    ++exp;
                }
                if (exp < 64 && fib_is[exp] && exp > maxFib)
                    maxFib = exp;
            }
            add = (seq_t)maxFib;
        }
        cum_sum[k] = cum_sum[k - 1] + add;
    }
    cum_last = N;

    return cum_sum[N];
}
