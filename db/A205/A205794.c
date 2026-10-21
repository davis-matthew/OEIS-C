/*
 * A205794  Least positive integer j such that n divides C(k)-C(j) , where k,
 *          as in A205793, is the least number for which there is such a j,
 *          and C=A002808 (composite numbers).
 * https://oeis.org/A205794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A205794: For each positive integer n, let C(k) be the k‑th composite
 * number (C(1)=4, C(2)=6, C(3)=8, …).  Find the smallest index k such that
 * there exists j<k with n | (C(k)-C(j)).  Then a(n) is the smallest such
 * j.  Equivalently, a(n) is the first index at which a residue modulo n
 * repeats among the composite numbers C(1),C(2),…  The value a(n) is
 * always <= k-1 <= n.                                              */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* maximum n that will be queried (enough for n <= 10000) */
#define MAX_N 10000
/* maximum number of composite indices we precompute */
#define MAX_K 11000
/* upper bound for sieving primes to get at least MAX_K composites */
#define SIEVE_UP 200000

/* static arrays for simple indexing */
static seq_t *comps = NULL;           /* comps[1..MAX_K] holds composite values */
static bool   initialized = false;

/* simple Eratosthenes sieve that also produces the first MAX_K composites */
static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* allocate composite array */
    comps = (seq_t*)malloc((MAX_K + 1) * sizeof(seq_t));
    if (!comps) abort();

    /* sieve for primes up to SIEVE_UP */
    bool *is_prime = (bool*)malloc((SIEVE_UP + 1) * sizeof(bool));
    if (!is_prime) abort();

    for (int i = 0; i <= SIEVE_UP; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; (long long)i * i <= SIEVE_UP; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= SIEVE_UP; j += i)
                is_prime[j] = false;
        }
    }

    /* collect composites */
    int cnt = 0;
    for (int num = 4; num <= SIEVE_UP && cnt < MAX_K; ++num) {
        if (!is_prime[num]) {
            ++cnt;
            comps[cnt] = (seq_t)num;
        }
    }
    /* free sieve memory */
    free(is_prime);
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, but guard anyway */

    init(); /* ensure composites ready */

    idx_t nb = (idx_t)n;
    if (nb > MAX_N) {
        /* For safety, handle larger n by allocating a temporary array */
        /* This case is unlikely in the intended test range. */
        int *first = (int*)calloc((size_t)nb, sizeof(int));
        if (!first) abort();

        int result = 0;
        for (int i = 1; ; ++i) {
            /* make sure we have enough composites computed */
            if (i > MAX_K) {
                /* grow comps array if necessary (not expected) */
                abort();
            }
            int residue = (int)((idx_t)comps[i] % nb);
            if (first[residue] == 0)
                first[residue] = i;
            else {
                result = first[residue];
                break;
            }
        }
        free(first);
        return (seq_t)result;
    }

    /* static array for residues; only using up to n */
    static int first[MAX_N + 1];
    for (int r = 0; r < (int)nb; ++r) first[r] = 0;

    for (int i = 1; ; ++i) {
        /* we have at least nb+1 composites due to pigeonhole principle */
        if (i > MAX_K) abort(); /* safety */
        int residue = (int)((idx_t)comps[i] % nb);
        if (first[residue] == 0)
            first[residue] = i;
        else
            return (seq_t)first[residue];
    }
}
