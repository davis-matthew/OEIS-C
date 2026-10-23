/*
 * A124317  Semiprimes indexed by 3-almost primes.
 * https://oeis.org/A124317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 4023, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A124317: a(n) = semiprime(3-almostprime(n)) */
/* Implemented by precomputing a list of 3‑almost primes and a list
   of semiprimes via a sieve for the smallest prime factor.          */

#include <stdlib.h>   /* malloc, free */
#include <stdbool.h>  /* bool type */

/* static state for memoisation */
static bool initialized = false;
static seq_t *three_almost = NULL;  /* 3‑almost prime values         */
static size_t three_len   = 0;
static seq_t *semiprimes  = NULL;   /* semiprime values               */
static size_t semi_len    = 0;

/* ------- helper: pre‑compute once -------- */
static void precompute(void)
{
    if (initialized) return;
    initialized = true;

    /* ---- parameters ---- */
    const idx_t LIMIT = 1000000;          /* enough for n up to 10 000   */
    const size_t MAX_3ALMOST = 20000;     /* safety buffer              */
    const size_t MAX_SEMI = 200000;       /* safety buffer              */

    /* ----- step 1: linear sieve for spf ----- */
    idx_t *spf = (idx_t*)malloc((SIZE_MAX > (LIMIT+1) ? LIMIT+1 : LIMIT)*sizeof(idx_t));
    if (!spf) return;
    for (idx_t i = 0; i <= LIMIT; ++i) spf[i] = 0;

    /* primes array allocated larger than needed (limit/10 enough) */
    size_t pcnt = 0;
    idx_t *primes = (idx_t*)malloc((LIMIT / 10 + 1) * sizeof(idx_t));
    if (!primes) { free(spf); return; }

    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {        /* i is prime */
            spf[i] = i;
            primes[pcnt++] = i;
        }
        for (size_t j = 0; j < pcnt; ++j) {
            idx_t p = primes[j];
            if (p > spf[i] || i * p > LIMIT) break;
            spf[i * p] = p;
        }
    }

    /* ----- step 2: compute Omega (count of prime factors with multiplicity) ----- */
    unsigned char *omega = (unsigned char*)malloc((LIMIT + 1) * sizeof(unsigned char));
    if (!omega) { free(spf); free(primes); return; }
    omega[1] = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        omega[i] = (unsigned char)(1 + omega[i / spf[i]]);

    /* ----- step 3: list 3‑almost primes ----- */
    three_almost = (seq_t*)malloc(MAX_3ALMOST * sizeof(seq_t));
    if (!three_almost) { free(omega); free(primes); free(spf); return; }
    three_len = 0;
    for (idx_t i = 2; i <= LIMIT && three_len < MAX_3ALMOST; ++i)
        if (omega[i] == 3)
            three_almost[three_len++] = (seq_t)i;

    /* ----- step 4: list semiprimes ----- */
    semiprimes = (seq_t*)malloc(MAX_SEMI * sizeof(seq_t));
    if (!semiprimes) { free(omega); free(primes); free(spf); return; }
    semi_len = 0;
    for (idx_t i = 2; i <= LIMIT && semi_len < MAX_SEMI; ++i)
        if (omega[i] == 2)
            semiprimes[semi_len++] = (seq_t)i;

    /* ----- cleanup temporary arrays ----- */
    free(omega);
    free(primes);
    free(spf);
}

/* ------- the required function ------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is 1‑based   */
    precompute();                  /* ensure tables exist   */

    if (n > three_len) return 0;   /* out of pre‑computed range */

    seq_t third_val = three_almost[n - 1];      /* n‑th 3‑almost prime   */
    idx_t idx = (idx_t)third_val;              /* its value as index    */

    if (idx == 0 || idx > (idx_t)semi_len) return 0;  /* safety */

    return semiprimes[idx - 1];    /* idx‑th semiprime       */
}
