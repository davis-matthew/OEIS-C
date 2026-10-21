/*
 * A055767  Index (or subscript) k of the largest primorial A002110(k) that
 *          divides the Euler phi of the n-th primorial (A005867(n)).
 * https://oeis.org/A055767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest k such that the primorial of index k divides
 *            phi(primorial of index n)
 * Range:     u8 n <= 3942, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* The build supplies the integer types idx_t and seq_t. */
/* ------------------------------------------------------------------ */
/* Upper bounds – adjust if a larger range of n is required. */
#define LIMIT     200000          /* maximum prime value needed          */
#define MAX_PRIMES 20000          /* safe upper bound on number of primes */

/* ------------------------------------------------------------------ */
/* Prime sieve data */
static int  primes[MAX_PRIMES];
static int  primeCount = 0;
static uint32_t primeIndex[LIMIT + 1];   /* 0 for composite, otherwise 1‑based index */

/* ------------------------------------------------------------------ */
/* Sequence storage */
static seq_t *aArr = NULL;          /* aArr[n] holds a(n) for 1 ≤ n ≤ primeCount */
static bool *seen  = NULL;          /* seen[k] is true if the k‑th prime has appeared */

/* ------------------------------------------------------------------ */
/* One‑time initialization */
static void init(void)
{
    if (aArr) return;                      /* already initialized */

    /* ----- Build sieve and prime list ----- */
    bool *is_prime = (bool *)calloc(LIMIT + 1, sizeof(bool));
    for (int i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (int i = 2; i * i <= LIMIT; ++i) {
        if (!is_prime[i]) continue;
        for (int j = i * i; j <= LIMIT; j += i) is_prime[j] = false;
    }

    for (int i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            if (primeCount < MAX_PRIMES) primes[primeCount++] = i;
            primeIndex[i] = primeCount;     /* 1‑based index */
        }
    }
    free(is_prime);

    /* ----- Allocate sequence and helper arrays ----- */
    aArr = (seq_t *)malloc((primeCount + 2) * sizeof(seq_t));
    seen = (bool *)calloc(primeCount + 2, sizeof(bool));

    /* ----- Compute the sequence incrementally ----- */
    int current_k = 0;                    /* largest contiguous prefix satisfied so far */

    for (int n = 1; n <= primeCount; ++n) {
        int p = primes[n - 1];            /* n‑th prime (0‑based in primes[]) */
        int t = p - 1;

        /* Factor t and mark the corresponding prime indices */
        for (int i = 0; i < primeCount && (long long)primes[i] * primes[i] <= t; ++i) {
            int q = primes[i];
            if (t % q == 0) {
                int idx = primeIndex[q];
                seen[idx] = true;
                while (t % q == 0) t /= q;
            }
        }
        if (t > 1) {                      /* remaining prime factor */
            int idx = primeIndex[t];
            seen[idx] = true;
        }

        /* Extend current_k while the next index is satisfied */
        while (current_k < primeCount && seen[current_k + 1]) current_k++;

        aArr[n] = (seq_t)current_k;
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the harness */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* OEIS offset is 1 */
    init();                               /* ensure data are ready */
    if (n > (idx_t)primeCount) return (seq_t)0; /* out of precomputed range */
    return aArr[(int)n];
}
