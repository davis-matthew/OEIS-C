/*
 * A106144  Semiprimes (mod 8).
 * https://oeis.org/A106144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A106144
    a(n) = (p_n) mod 8
    where p_n is the n‑th semiprime (product of exactly two primes,
    counting multiplicity).  This implementation generates the
    semiprimes incrementally using a least‑prime‑factor sieve and
    stores their modulo‑8 values.  It works for any seq_t and idx_t
    defined by the build.  For arbitrary‑precision seq_t the code
    casts the result of calloc/realloc to the appropriate pointer
    type so it compiles under C++17. */

#include <stdlib.h>

/* ---------- Least‑prime‑factor sieve (dynamic, O(n log log n) ) --------- */
static idx_t spf_bound = 0;          /* highest integer whose spf is known */
static unsigned int *spf = NULL;     /* least prime factor for each i */

/* Ensure spf covers at least [2, min_n] */
static void ensure_bound(idx_t min_n)
{
    if (min_n <= spf_bound) return;

    /* grow by at least doubling to keep amortised cost low */
    idx_t new_bound = spf_bound ? spf_bound * 2 : 4;
    if (new_bound < min_n) new_bound = min_n;

    unsigned int *new_spf = (unsigned int *)calloc((size_t)new_bound + 1,
                                                   sizeof(unsigned int));
    if (!new_spf) abort();          /* memory allocation failure */

    for (idx_t i = 2; i <= new_bound; ++i) {
        if (!new_spf[i]) {           /* i is prime */
            new_spf[i] = (unsigned int)i;
            if (i > new_bound / i) continue;   /* avoid overflow */
            for (idx_t j = i * i; j <= new_bound; j += i)
                if (!new_spf[j]) new_spf[j] = (unsigned int)i;
        }
    }
    free(spf);
    spf = new_spf;
    spf_bound = new_bound;
}

/* Test whether x is a semiprime (exactly two prime factors, counted
   with multiplicity).  Requires spf to be defined up to x. */
static int is_semiprime(idx_t x)
{
    if (x < 4) return 0;
    int cnt = 0;
    idx_t tmp = x;
    while (tmp > 1 && cnt <= 2) {
        unsigned int p = spf[(size_t)tmp];
        tmp /= p;
        ++cnt;
    }
    return (cnt == 2 && tmp == 1);
}

/* ---------- Table of semiprimes modulo 8 --------------------------- */
static seq_t *sem_mod8 = NULL;   /* array of stored modulo‑8 values */
static idx_t sem_cap   = 0;      /* current allocation capacity */
static idx_t sem_size  = 0;      /* number of semiprimes found so far */
static idx_t current_candidate = 4;   /* next integer to test */

/* Ensure capacity for at least 'min_needed' entries. */
static void ensure_sem_capacity(idx_t min_needed)
{
    if (sem_cap >= min_needed) return;
    idx_t new_cap = sem_cap ? sem_cap * 2 : 1;
    while (new_cap < min_needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(sem_mod8, (size_t)new_cap * sizeof(seq_t));
    if (!tmp) abort();
    sem_mod8 = tmp;
    sem_cap  = new_cap;
}

/* ---------- Public sequence function --------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined below offset */

    /* Grow the list until we have at least n semiprimes. */
    while (sem_size < n) {
        /* Ensure the sieve covers the current candidate. */
        ensure_bound(current_candidate);

        if (is_semiprime(current_candidate)) {
            seq_t val = (seq_t)(current_candidate % (idx_t)8);
            ensure_sem_capacity(sem_size + 1);
            sem_mod8[sem_size++] = val;
        }
        ++current_candidate;
    }

    return sem_mod8[n - 1];
}
