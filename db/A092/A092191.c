/*
 * A092191  Numbers n such that sum of n-th and (n+1)-th semiprimes is a
 *          semiprime.
 * https://oeis.org/A092191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the index n of consecutive semiprimes whose sum is a semiprime.
 * Computation: Generate all semiprimes up to a fixed bound, then scan successive pairs.
 * Whenever s[n] + s[n+1] is semiprime we store the 1‑based index n.
 * The first 20000 indices are pre‑computed; this covers all tests up to n≈10000. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t   *qual_arr   = NULL;   /* stored qualifying indices (1‑based)   */
static size_t  qual_len    = 0;      /* number of filled elements            */
static const size_t  QUAL_TARGET = 20000; /* safety margin for required terms      */
static bool     initialized = false;

/* Pre‑compute semiprimes and qualifying indices */
static void init_sequence(void)
{
    if (initialized) return;
    initialized = true;

    /* Limit for the semiprime list (enough to obtain >20000 qualifying indices).  */
    const idx_t SEM_LIMIT = 500000;           /* semiprimes to generate            */
    const idx_t MAX_VAL  = SEM_LIMIT * 2;      /* maximum value needed for sums      */

    const size_t SZ = (size_t)MAX_VAL + 1;

    /* Allocate auxiliary arrays */
    unsigned char *is_semiprime = (unsigned char*)malloc(SZ);
    unsigned char *fact_cnt     = (unsigned char*)malloc(SZ);
    int           *spf           = (int*)malloc(SZ * sizeof(int));
    int           *primes        = (int*)malloc(SZ * sizeof(int));  /* prime list for sieve */

    /* Linear sieve to compute smallest prime factor (spf) and prime list */
    size_t prime_cnt = 0;
    for (size_t i = 0; i < SZ; ++i) spf[i] = 0;
    for (int i = 2; i <= MAX_VAL; ++i)
    {
        if (!spf[i]) {
            spf[i] = i;
            primes[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            int p = primes[j];
            long long prod = (long long)i * p;
            if (prod > MAX_VAL) break;
            spf[prod] = p;
            if (p == spf[i]) break;
        }
    }

    /* Count prime factors with multiplicity and tag semiprimes (factor count = 2) */
    fact_cnt[1] = 0;
    for (int i = 2; i <= MAX_VAL; ++i) {
        fact_cnt[i] = fact_cnt[i / spf[i]] + 1;
        is_semiprime[i] = (fact_cnt[i] == 2) ? 1 : 0;
    }

    /* Build list of semiprimes up to SEM_LIMIT */
    size_t sem_cap = SEM_LIMIT / 10 + 10;
    idx_t *sem_vals = (idx_t*)malloc(sem_cap * sizeof(idx_t));
    size_t sem_cnt = 0;
    for (int i = 4; i <= SEM_LIMIT; ++i) {
        if (is_semiprime[i]) {
            if (sem_cnt >= sem_cap) {
                sem_cap *= 2;
                sem_vals = (idx_t*)realloc(sem_vals, sem_cap * sizeof(idx_t));
            }
            sem_vals[sem_cnt++] = (idx_t)i;
        }
    }

    /* Scan successive pairs and collect qualifying indices */
    size_t qual_cap = QUAL_TARGET * 2;
    qual_arr = (seq_t*)malloc(qual_cap * sizeof(seq_t));

    for (size_t idx = 0; idx + 1 < sem_cnt; ++idx) {
        idx_t s1 = sem_vals[idx];
        idx_t s2 = sem_vals[idx + 1];
        idx_t sum = s1 + s2;
        if (sum <= MAX_VAL && is_semiprime[sum]) {
            if (qual_len >= qual_cap) {
                qual_cap *= 2;
                qual_arr = (seq_t*)realloc(qual_arr, qual_cap * sizeof(seq_t));
            }
            qual_arr[qual_len++] = (seq_t)(idx + 1);  /* 1‑based index of semiprime n */
            if (qual_len >= QUAL_TARGET) break;      /* we have enough terms */
        }
    }

    /* Free temporary memory */
    free(is_semiprime);
    free(fact_cnt);
    free(spf);
    free(primes);
    free(sem_vals);
}

/* Return the n‑th term of the sequence A092191 */
seq_t a(idx_t n)
{
    init_sequence();
    if (n == 0 || n > (idx_t)qual_len) return 0;
    return qual_arr[n - 1];
}
