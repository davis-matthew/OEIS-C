/*
 * A091373  Number of numbers <= n having exactly as many prime factors as the
 *          value of their smallest prime factor.
 * https://oeis.org/A091373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{m : Ω(m)=spf(m), m ≤ n}. The function builds a sieve up
 *            to the requested n, stores the smallest prime factor (spf) and
 *            the big omega (Ω) for each number, and builds a prefix
 *            enumeration. a(n) is the n‑th item of this prefix array
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static state shared between calls */
static idx_t *spf = NULL;   /* smallest prime factor for each index */
static int *bigomega = NULL; /* Ω(n) = total number of prime factors */
static seq_t *pref = NULL;  /* prefix counts a(i) for i = 0..size-1 */
static size_t cur_size = 0; /* current allocated size */

/* extend the tables to cover indices up to n (inclusive) */
static void ensure_upto(idx_t n)
{
    size_t N = (size_t)n + 1;
    if (cur_size >= N)
        return; /* already computed */

    /* reallocate arrays */
    spf = (idx_t *)realloc(spf, N * sizeof(idx_t));
    bigomega = (int *)realloc(bigomega, N * sizeof(int));
    pref = (seq_t *)realloc(pref, N * sizeof(seq_t));

    /* initialise spf: for i>=2 set spf[i] = i initially */
    for (size_t i = 0; i < N; ++i)
        spf[i] = (idx_t)i;

    /* sieve for smallest prime factor */
    for (size_t i = 2; i * i < N; ++i) {
        if (spf[i] == (idx_t)i) {          /* i is prime */
            for (size_t j = i * i; j < N; j += i) {
                if (spf[j] == (idx_t)j)
                    spf[j] = (idx_t)i;
            }
        }
    }

    /* compute bigomega using the spf */
    bigomega[0] = 0;
    if (N > 1)
        bigomega[1] = 0;
    for (size_t i = 2; i < N; ++i) {
        bigomega[i] = bigomega[i / (size_t)spf[i]] + 1;
    }

    /* build prefix counts */
    pref[0] = 0;
    if (N > 1)
        pref[1] = 0;
    for (size_t i = 2; i < N; ++i) {
        pref[i] = pref[i - 1] +
                  ((bigomega[i] == (int)spf[i]) ? (seq_t)1 : (seq_t)0);
    }

    cur_size = N;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined for n < 1; return 0 */
    ensure_upto(n);
    return pref[(size_t)n];
}
