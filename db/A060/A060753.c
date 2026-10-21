/*
 * A060753  Denominator of 1*2*4*6*...*(prime(n-1)-1) /
 *          (2*3*5*7*...*prime(n-1)).
 * https://oeis.org/A060753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = product of the first n‑1 primes that do NOT divide any of
 *            the numbers (prime(k) – 1) for k < n. The function computes a(n)
 *            using that characterization, storing intermediate results for
 *            fast repeated calls
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_PRIME 200000          /* primes up to this bound cover n ≤ 10 000   */
#define INITIAL_CAP 2             /* initial capacity for the value table      */

static bool initialized = false;

/* list of primes (0‑based) */
static uint64_t primes[MAX_PRIME];
static int prime_count = 0;

/* blocked[p] == true  if prime p has already appeared as a factor of
   some (prime(k) – 1) and therefore is removed from the running
   denominator product. */
static bool blocked[MAX_PRIME + 1];

/* cache of sequence values: a_vals[1] .. a_vals[current_max_n] */
static seq_t *a_vals = NULL;
static size_t a_cap = 0;       /* allocated capacity (>= current_max_n)      */
static idx_t current_max_n = 1;/* last index that has been computed          */

/* running product of the primes currently not blocked */
static seq_t curprod;

/* ------------------------------------------------------------ */
/*  Simple sieve to generate all primes up to MAX_PRIME          */
/* ------------------------------------------------------------ */
static void init_primes(void)
{
    bool *sieve = (bool*)malloc((MAX_PRIME + 1) * sizeof(bool));
    int i, j;
    for (i = 0; i <= MAX_PRIME; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (i = 2; i * i <= MAX_PRIME; ++i)
        if (sieve[i])
            for (j = i * i; j <= MAX_PRIME; j += i)
                sieve[j] = false;

    prime_count = 0;
    for (i = 2; i <= MAX_PRIME; ++i)
        if (sieve[i])
            primes[prime_count++] = (uint64_t)i;
    free(sieve);
}

/* ------------------------------------------------------------ */
/*  Initialise global tables and first value of the sequence   */
/* ------------------------------------------------------------ */
static void init(void)
{
    init_primes();

    /* blocked array */
    for (int i = 0; i <= MAX_PRIME; ++i) blocked[i] = false;

    /* allocate value array */
    a_cap = INITIAL_CAP;
    a_vals = (seq_t*)malloc((a_cap + 1) * sizeof(seq_t));
    a_vals[1] = (seq_t)1;         /* a(1) = 1 */
    a_cap = INITIAL_CAP;

    /* running product starts at 1 */
    curprod = (seq_t)1;
    current_max_n = 1;

    initialized = true;
}

/* ------------------------------------------------------------ */
/*  Expand the allocated array when necessary                  */
/* ------------------------------------------------------------ */
static void ensure_capacity(size_t needed)
{
    if (a_cap < needed) {
        size_t new_cap = a_cap;
        while (new_cap < needed) new_cap <<= 1;

        seq_t *tmp = (seq_t*)realloc(a_vals, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* unrecoverable allocation error */
        a_vals = tmp;
        a_cap = new_cap;
    }
}

/* ------------------------------------------------------------ */
/*  Compute a(n).  The function is memoised and can be called
   in any order.                                                 */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n < 1)                 /* below offset behaviour is irrelevant */
        return (seq_t)1;

    ensure_capacity(n);

    /* Compute missing terms up to n */
    while (current_max_n < n) {
        /* prime index for the current step: p_{current_max_n} */
        uint64_t p = primes[current_max_n - 1];   /* zero‑based index */

        /* If this prime is not yet blocked, include it */
        if (!blocked[(size_t)p])
            curprod *= (seq_t)p;

        /* Factor p – 1 into its distinct prime factors */
        uint64_t m = p - 1;
        uint64_t fac[20];   /* p – 1 < 200000, so fewer than 20 distinct factors */
        int fcnt = 0;
        for (int i = 0; i < prime_count && (uint64_t)primes[i] * (uint64_t)primes[i] <= m; ++i) {
            uint64_t pr = primes[i];
            if (m % pr == 0) {
                fac[fcnt++] = pr;
                while (m % pr == 0) m /= pr;
            }
        }
        if (m > 1) fac[fcnt++] = m;

        /* For each distinct factor, block it and remove it from curprod
           if it was previously present.  All factors are < p, thus
           already processed. */
        for (int k = 0; k < fcnt; ++k) {
            uint64_t q = fac[k];
            if (!blocked[(size_t)q]) {
                blocked[(size_t)q] = true;
                curprod /= (seq_t)q;    /* exact division – q divides curprod */
            }
        }

        /* Store the value for a(current_max_n + 1) */
        a_vals[current_max_n + 1] = curprod;
        ++current_max_n;
    }

    return a_vals[n];
}
