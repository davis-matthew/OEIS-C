/*
 * A179213  Sum of squarefree numbers between n and 2*n (inclusive).
 * https://oeis.org/A179213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 268, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=n}^{2*n} k*A008966(k)
 *          where A008966(k)=1 iff k is squarefree.
 * The function precomputes a prefix sum of k for all squarefree k up to the
 * requested 2*n and then returns pref[2*n]-pref[n-1].
 *
 * It is memoized so that repeated calls with arbitrary n are fast.
 * The implementation uses only integer arithmetic suitable for both
 * fixed‑width and arbitrary‑precision builds, and works in C11/C++17.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static idx_t max_n = 0;        /* current maximum index for which we have data */
static seq_t *pref = NULL;     /* prefix sums: pref[k] = sum_{i=1}^k i*sf(i) */
static bool *is_sqfree = NULL; /* flag array for squarefreeness */
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;

/* Generate primes up to sqrt(max_n) */
static void build_primes_up_to(idx_t limit)
{
    /* simple sieve up to 'limit' */
    bool *comp = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    memset(comp, 0, (size_t)(limit + 1) * sizeof(bool));

    /* allocate maximum possible number of primes ~ limit/log(limit) */
    size_t est = (size_t)(limit / (limit > 1 ? (log((double)limit)+1) : 1) + 10);
    uint32_t *pr = (uint32_t *)malloc(est * sizeof(uint32_t));
    size_t cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!comp[i]) {
            if (cnt == est) {
                est *= 2;
                pr = (uint32_t *)realloc(pr, est * sizeof(uint32_t));
            }
            pr[cnt++] = (uint32_t)i;
            if ((uint64_t)i * i <= (uint64_t)limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    comp[j] = 1;
            }
        }
    }

    free(comp);
    free(primes);
    primes = pr;
    primes_cnt = cnt;
}

/* Test whether k is squarefree using the current prime list */
static bool is_squarefree_k(idx_t k)
{
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t p2 = (uint64_t)p * (uint64_t)p;
        if (p2 > (uint64_t)k)
            break;
        if (k % p == 0) {
            if ((k / p) % p == 0)
                return false;          /* p^2 divides k */
        }
    }
    return true;
}

/* Ensure that data up to 'N' is available */
static void ensure_prefix(idx_t N)
{
    if (N <= max_n)
        return;

    idx_t old_max = max_n;
    idx_t new_max = N;
    max_n = new_max;

    /* Reallocate prefix and squarefree arrays */
    pref = (seq_t *)realloc(pref, (size_t)(new_max + 1) * sizeof(seq_t));
    is_sqfree = (bool *)realloc(is_sqfree,
                                (size_t)(new_max + 1) * sizeof(bool));

    if (old_max == 0) {
        pref[0] = 0;
    }

    /* Build prime list up to sqrt(new_max) */
    idx_t sqrt_lim = (idx_t)floor(sqrt((double)new_max));
    build_primes_up_to(sqrt_lim);

    /* Compute data from old_max+1 to new_max */
    for (idx_t k = old_max + 1; k <= new_max; ++k) {
        bool sf = is_squarefree_k(k);
        is_sqfree[k] = sf;
        pref[k] = pref[k - 1] + (sf ? (seq_t)k : (seq_t)0);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                  /* offset 1; behaviour below 1 is irrelevant */

    idx_t upto = 2 * n;
    ensure_prefix(upto);
    return pref[upto] - pref[n - 1];
}
