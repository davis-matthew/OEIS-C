/*
 * A079592  Primes p such that floor(Pi*p) is prime.
 * https://oeis.org/A079592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A079592 – Primes p for which floor(π·p) is prime */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* All indices are taken 1‑based: a(1) is the first term of the OEIS list. */
static bool built = false;
static seq_t *qual = NULL;          /* qualifiers, stored in order */
static idx_t  qual_len = 0;         /* number of qualifiers */
static const int LIMIT_PRIME = 13000000;   /* sieve limit (covers floor(π·p) for p≤2·10⁶) */
static const int LIMIT_P     = 2000000;    /* maximal prime considered for p */
static char *prime_mark = NULL;      /* sieve array */

/* Simple but fast sieve, returning a boolean array up to LIMIT. */
static void build(void)
{
    /* 1.  Allocate sieve and initialise as all 1 */
    prime_mark = (char *)calloc((size_t)LIMIT_PRIME + 1, sizeof(char));
    if (!prime_mark) exit(1);   /* out of memory */
    for (int i = 2; i <= LIMIT_PRIME; ++i) prime_mark[i] = 1;

    /* 2.  Eratosthenes sieve */
    int sq = (int)sqrt((double)LIMIT_PRIME);
    for (int i = 2; i <= sq; ++i)
        if (prime_mark[i])
            for (int j = i * i; j <= LIMIT_PRIME; j += i)
                prime_mark[j] = 0;

    /* 3.  Build the list of primes up to LIMIT_P (used for the outer loop) */
    int prime_cnt = 0;
    for (int i = 2; i <= LIMIT_P; ++i) if (prime_mark[i]) ++prime_cnt;
    int *primes = (int *)malloc((size_t)prime_cnt * sizeof(int));
    if (!primes) exit(1);
    for (int i = 2, k = 0; i <= LIMIT_P; ++i)
        if (prime_mark[i]) primes[k++] = i;

    /* 4.  For each prime p, test if floor(π·p) is prime */
    const long double pi = acosl(-1.0L);
    idx_t temp_cap = 0;
    seq_t *temp = NULL;

    for (int k = 0; k < prime_cnt; ++k) {
        int p = primes[k];
        unsigned long long val = (unsigned long long)(pi * (long double)p);
        if (val <= (unsigned long long)LIMIT_PRIME && prime_mark[(int)val]) {
            /* store the qualifying prime */
            if (qual_len == temp_cap) {
                temp_cap = temp_cap ? temp_cap << 1 : 32;
                temp = (seq_t *)realloc(temp, (size_t)temp_cap * sizeof(seq_t));
                if (!temp) exit(1);
            }
            temp[qual_len++] = (seq_t)p;
        }
    }

    /* 5.  finalize */
    free(primes);
    qual = temp;         /* take ownership */
    built = true;
}

seq_t a(idx_t n)
{
    if (!built) build();
    /* the sequence is 1‑based; the test harness will never ask for n ≤ 0 */
    return qual[n - 1];
}
