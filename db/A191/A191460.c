/*
 * A191460  Expansion of e.g.f.: A(x) = Sum_{n>=0} binomial(x^n, n).
 * https://oeis.org/A191460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{d|n} s(d,n/d) * n! / d! where s(.,.) is the signed
 *            Stirling number of the first kind
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 13, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Signed Stirling numbers of the first kind (memoised)               */
/* ------------------------------------------------------------------ */

typedef struct {
    uint64_t key;   /* (n << 32) | k  */
    seq_t    val;
    char     used;
} StirEntry;

static StirEntry *stir_tbl    = NULL;
static size_t    stir_cap    = 0;
static size_t    stir_used   = 0;

static void stir_init(void)
{
    if (stir_tbl) return;
    stir_cap = 1 << 14;               /* 16384 slots, power of 2 */
    stir_tbl = (StirEntry *)calloc(stir_cap, sizeof(StirEntry));
}

static void stir_expand(void)
{
    size_t new_cap = stir_cap << 1;
    StirEntry *new_tbl = (StirEntry *)calloc(new_cap, sizeof(StirEntry));

    for (size_t i = 0; i < stir_cap; ++i) {
        if (stir_tbl[i].used) {
            uint64_t key = stir_tbl[i].key;
            seq_t    val = stir_tbl[i].val;
            size_t h = key & (new_cap - 1);
            while (new_tbl[h].used) h = (h + 1) & (new_cap - 1);
            new_tbl[h].key = key;
            new_tbl[h].val = val;
            new_tbl[h].used = 1;
        }
    }
    free(stir_tbl);
    stir_tbl = new_tbl;
    stir_cap = new_cap;
}

/* recursive signed Stirling1(n,k) with memoisation */
static seq_t stir_signed(idx_t n, idx_t k)
{
    if (k == 0) return (n == 0) ? (seq_t)1 : (seq_t)0;
    if (k > n)   return (seq_t)0;

    uint64_t key = ((uint64_t)n << 32) | k;
    stir_init();

    size_t h = key & (stir_cap - 1);
    while (stir_tbl[h].used) {
        if (stir_tbl[h].key == key) return stir_tbl[h].val;
        h = (h + 1) & (stir_cap - 1);
    }

    seq_t val;
    if (n == k) {
        val = (seq_t)1;
    } else {
        /* s(n,k) = s(n-1,k-1) - (n-1)*s(n-1,k) */
        val = stir_signed(n-1, k-1) - ((seq_t)(n-1) * stir_signed(n-1, k));
    }

    /* store result */
    while (stir_tbl[h].used) h = (h + 1) & (stir_cap - 1);
    stir_tbl[h].key = key;
    stir_tbl[h].val = val;
    stir_tbl[h].used = 1;
    if (++stir_used * 10 > stir_cap * 6) stir_expand();

    return val;
}

/* ------------------------------------------------------------------ */
/*  Factorial table (up‑to current maximum n)                          */
/* ------------------------------------------------------------------ */

static seq_t *fact_tbl = NULL;
static idx_t  fact_sz  = 0;

static void ensure_factorial(idx_t n)
{
    if (fact_sz > n) return;

    if (fact_sz == 0) {
        fact_tbl = (seq_t *)malloc(sizeof(seq_t));
        fact_tbl[0] = (seq_t)1;
        fact_sz = 1;
    }

    if (n + 1 <= fact_sz) return;

    seq_t *tmp = (seq_t *)realloc(fact_tbl, (n + 1) * sizeof(seq_t));
    if (!tmp) exit(1);               /* out of memory: abort early */
    fact_tbl = tmp;

    for (idx_t i = fact_sz; i <= n; ++i) {
        fact_tbl[i] = fact_tbl[i-1] * (seq_t)i;
    }
    fact_sz = n + 1;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                             */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    ensure_factorial(n);
    seq_t fact_n = fact_tbl[n];

    seq_t result = (seq_t)0;

    /* enumerate divisors up to sqrt(n) */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i != 0) continue;

        /* first divisor d1 = i */
        idx_t d1 = i;
        idx_t k1 = n / d1;
        if (k1 <= d1) {
            seq_t s1 = stir_signed(d1, k1);
            seq_t coeff1 = fact_n / fact_tbl[d1];
            result += s1 * coeff1;
        }

        /* complementary divisor d2 = n/i */
        idx_t d2 = n / i;
        if (d2 == d1) continue;          /* square root handled already */

        idx_t k2 = n / d2;                /* equals d1 */
        if (k2 <= d2) {
            seq_t s2 = stir_signed(d2, k2);
            seq_t coeff2 = fact_n / fact_tbl[d2];
            result += s2 * coeff2;
        }
    }

    return result;
}
