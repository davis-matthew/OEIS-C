/*
 * A061775  Number of nodes in rooted tree with Matula-Goebel number n.
 * https://oeis.org/A061775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   It satisfies the recurrence a(1) = 1; if n = p_t (the t‑th
 *            prime) then a(n) = 1 + a(t); if n = u v (with u,v ≥ 2) then a(n)
 *            = a(u) + a(v) – 1. The function memoises results and builds a
 *            sieve of smallest prime factors on demand
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The build supplies the types seq_t and idx_t.  We do not declare them here. */

static idx_t *spf          = NULL;     /* smallest prime factor of i  */
static idx_t *prime_index  = NULL;     /* 1‑based index of prime i (0 if composite) */
static size_t sieve_limit  = 0;         /* largest n for which spf/prime_index are valid */

static seq_t *a_arr        = NULL;     /* memoised sequence values up to a_limit */
static size_t a_limit      = 0;         /* largest n whose a(n) has been computed */

/* Build or extend the sieve up to “limit”.  All entries ≤ limit
   will have spf[i] set (primes have spf[i]==i) and prime_index[i]
   equal to the 1‑based index of i when i is prime. */
static void build_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;   /* nothing to do */

    size_t sz = (size_t)limit + 1;
    idx_t *new_spf = (idx_t *)realloc(spf, sz * sizeof(idx_t));
    idx_t *new_idx = (idx_t *)realloc(prime_index, sz * sizeof(idx_t));
    if (!new_spf || !new_idx) {
        /* allocation failure – callers will treat a missing value as 0 */
        return;
    }
    spf = new_spf;
    prime_index = new_idx;

    for (idx_t i = 0; i <= limit; ++i) {
        spf[i] = i;
        prime_index[i] = 0;
    }

    idx_t pc = 0;              /* current prime count */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == i) {     /* i is prime */
            ++pc;
            prime_index[i] = pc;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == j)
                        spf[j] = i;
            }
        }
    }
    sieve_limit = limit;
}

/* Ensure that a_arr contains a(j) for all 1 ≤ j ≤ n.
   Expands the array and computes any new values using the recurrence above. */
static void ensure_a(idx_t n)
{
    if (a_arr == NULL) {  /* first call */
        size_t sz = (size_t)n + 1;      /* use n+1 so that a_arr[n] is valid */
        a_arr = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!a_arr)
            return;            /* allocation failure – caller will see 0 */
        a_limit = 1;
        a_arr[1] = (seq_t)1;
        if (n == 1)
            return;
    }

    if (n > a_limit) {
        if (sieve_limit < n)
            build_sieve(n);

        size_t newsize = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(a_arr, newsize * sizeof(seq_t));
        if (!tmp) /* allocation failure – keep old data */
            return;
        a_arr = tmp;

        for (idx_t i = a_limit + 1; i <= n; ++i) {
            if (i == 1) {
                a_arr[1] = (seq_t)1;   /* already set, but keeps compiler happy */
            } else if (spf[i] == i) { /* prime */
                idx_t t = prime_index[i];
                a_arr[i] = (seq_t)1 + a_arr[t];
            } else {                 /* composite */
                idx_t u = spf[i];
                idx_t v = i / u;
                a_arr[i] = a_arr[u] + a_arr[v] - (seq_t)1;
            }
        }
        a_limit = n;
    }
}

/* External function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined; return 0 for safety */
    ensure_a(n);
    if (n > a_limit)          /* allocation failure handled in ensure_a */
        return (seq_t)0;
    return a_arr[n];
}
