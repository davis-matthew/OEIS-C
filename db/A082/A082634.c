/*
 * A082634  Numbers n which can be expressed as sum of d consecutive primes
 *          (where d>0 is a divisor of n).
 * https://oeis.org/A082634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate all sums of consecutive primes up to a limit
   and select those where the number of primes in the run divides the sum.
   The resulting list is the sequence A082634. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *seq_vals = NULL; /* array of sequence terms */
    static idx_t   seq_len = 0;    /* number of terms stored         */
    static idx_t   seq_limit = 0;  /* limit used for the current array */

    /* The sequence is defined only for n>=1.  If n==0, return 0. */
    if (n == 0) return 0;

    const idx_t INITIAL_LIMIT = 200000; /* starting limit (large enough for 10 000 terms) */

    /* Recompute the sequence until it contains at least n terms. */
    while (seq_len < n)
    {
        idx_t new_limit = (seq_limit == 0) ? INITIAL_LIMIT : seq_limit * 2;

        /* ---------- 1. Sieve primes up to new_limit ---------- */
        unsigned char *isPrime = (unsigned char *)malloc((size_t)(new_limit + 1));
        if (!isPrime) return 0;   /* allocation failed */
        memset(isPrime, 1, (size_t)(new_limit + 1));
        if (new_limit >= 0) isPrime[0] = 0;
        if (new_limit >= 1) isPrime[1] = 0;

        for (size_t p = 2; p * p <= (size_t)new_limit; ++p)
            if (isPrime[p])
                for (size_t q = p * p; q <= (size_t)new_limit; q += p)
                    isPrime[q] = 0;

        /* ---------- 2. Collect primes ---------- */
        int *primes = (int *)malloc(sizeof(int) * (new_limit + 1));
        if (!primes) { free(isPrime); return 0; }
        size_t primeCount = 0;
        for (size_t i = 2; i <= (size_t)new_limit; ++i)
            if (isPrime[i]) primes[primeCount++] = (int)i;

        /* ---------- 3. Prefix sums of primes ---------- */
        seq_t *pref = (seq_t *)malloc(sizeof(seq_t) * (primeCount + 1));
        if (!pref) { free(isPrime); free(primes); return 0; }
        pref[0] = 0;
        for (size_t i = 0; i < primeCount; ++i)
            pref[i + 1] = pref[i] + (seq_t)primes[i];

        /* ---------- 4. Determine qualifying sums ---------- */
        unsigned char *qual = (unsigned char *)malloc((size_t)(new_limit + 1));
        if (!qual) { free(isPrime); free(primes); free(pref); return 0; }
        memset(qual, 0, (size_t)(new_limit + 1));

        for (size_t start = 0; start < primeCount; ++start)
            for (size_t len = 1; start + len <= primeCount; ++len)
            {
                seq_t sum = pref[start + len] - pref[start];
                if (sum > (seq_t)new_limit) break;
                if (sum % (seq_t)len == 0)
                    qual[(size_t)sum] = 1;
            }

        /* ---------- 5. Build the sequence array ---------- */
        seq_t *new_seq = (seq_t *)malloc(sizeof(seq_t) * (new_limit + 1));
        if (!new_seq) {
            free(isPrime); free(primes); free(pref); free(qual);
            return 0;
        }
        idx_t new_len = 0;
        for (size_t val = 2; val <= (size_t)new_limit; ++val)
            if (qual[val]) new_seq[new_len++] = (seq_t)val;

        /* ---------- 6. Replace old data ---------- */
        if (seq_vals) free(seq_vals);
        seq_vals  = new_seq;
        seq_len   = new_len;
        seq_limit = new_limit;

        /* ---------- 7. Free temporary arrays ---------- */
        free(isPrime);
        free(primes);
        free(pref);
        free(qual);
    }

    /* Return the n-th term (1‑based indexing). */
    return seq_vals[n - 1];
}
