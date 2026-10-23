/*
 * A101595  a(1) = 2; a(n+1) is the least prime p not already used such that
 *          p-a(n) is not equal to a(k+1)-a(k) for any k < n.
 * https://oeis.org/A101595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A101595:
   a(1) = 2
   a(n+1) = the least unused prime p such that (p - a(n)) is not
   equal to any previous difference a(k+1)-a(k), k < n. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state */
    static bool initialized = false;
    static idx_t lastComputed = 0;          /* last index that has been computed */
    static seq_t *seq = NULL;               /* 1‑based array of sequence terms */
    static idx_t *primes = NULL;            /* list of all primes up to MAXP */
    static bool *primeUsed = NULL;          /* flag: prime already used?  */
    static bool *diffUsed = NULL;           /* flag: difference already seen? */
    static idx_t primeCnt = 0;              /* number of primes found          */

    /* constants --------------------------------------------------- */
    const idx_t MAXP   = 200000;            /* upper bound for prime sieve    */
    const idx_t DIFFSZ = 2 * MAXP + 1;      /* covers differences [-MAXP,MAXP]*/
    const idx_t MAXN   = 20000;             /* maximum number of stored terms */

    if (!initialized)
    {
        /* sieve of Eratosthenes up to MAXP */
        bool *isP = (bool*)calloc(MAXP + 1, sizeof(bool));
        for (idx_t i = 2; i <= MAXP; ++i) isP[i] = true;
        for (idx_t i = 2; i * i <= MAXP; ++i)
            if (isP[i])
                for (idx_t j = i * i; j <= MAXP; j += i)
                    isP[j] = false;

        /* count primes & build array */
        for (idx_t i = 2; i <= MAXP; ++i)
            if (isP[i]) ++primeCnt;
        primes = (idx_t*)malloc(primeCnt * sizeof(idx_t));
        for (idx_t i = 2, k = 0; i <= MAXP; ++i)
            if (isP[i]) primes[k++] = i;
        free(isP);

        /* auxiliary arrays */
        primeUsed = (bool*)calloc(primeCnt, sizeof(bool));
        diffUsed  = (bool*)calloc(DIFFSZ, sizeof(bool));
        seq       = (seq_t*)calloc(MAXN + 1, sizeof(seq_t));

        /* initialise first term */
        seq[1] = 2;
        /* mark prime 2 as used */
        for (idx_t i = 0; i < primeCnt; ++i)
            if (primes[i] == 2) { primeUsed[i] = true; break; }

        lastComputed = 1;
        initialized = true;
    }

    /* handle out‑of‑range n ------------------------------------------------ */
    if (n == 0) return 0;            /* offset starts at 1 */
    if (n > MAXN)
    {
        seq = (seq_t*)realloc(seq, (n + 1) * sizeof(seq_t));
    }

    /* memoisation: compute up to n ------------------------------------------- */
    for (idx_t cur = lastComputed + 1; cur <= n; ++cur)
    {
        seq_t prev = seq[cur - 1];
        idx_t prevIdx = (idx_t)prev; /* safe – all terms are primes ≤ MAXP */

        /* find least unused prime yielding a new difference */
        for (idx_t pi = 0; pi < primeCnt; ++pi)
        {
            if (primeUsed[pi]) continue;

            idx_t p = primes[pi];
            int diff = (int)((int64_t)p - (int64_t)prevIdx);
            if (diffUsed[(size_t)diff + MAXP]) continue;  /* difference already used */

            /* accept this prime */
            seq[cur]      = (seq_t)p;
            primeUsed[pi] = true;
            diffUsed[(size_t)diff + MAXP] = true;
            break;
        }
    }

    lastComputed = n;
    return seq[n];
}
