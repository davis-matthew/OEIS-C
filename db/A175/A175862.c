/*
 * A175862  a(n) = frequency of values n in A175856(m), where A175856(m): a(m)
 *          = m for m = noncomposites, a(m) = previous term - 1 for m =
 *          composites.
 * https://oeis.org/A175862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of non‑composite numbers p (primes or 1) for
 *            which n lies in the interval [ p – (nextPrime(p) – p – 1) , p ].
 *            Equivalently, a(n) equals the frequency of the value n in the
 *            sequence defined by A(m)=m if m is non‑composite, A(m)=A(m-1)-1
 *            if m is composite
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper: sieve primes up to 'limit', store in array 'primes'.
   The array 'primes' is preallocated with at least 'cap' elements.
   The function returns the number of primes found. */
static size_t build_primes(idx_t limit, idx_t *primes, size_t cap)
{
    size_t count = 0;
    bool *isComposite = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!isComposite) return 0;                /* allocation failure */

    for (idx_t p = 2; (idx_t)(p * p) <= limit; ++p) {
        if (!isComposite[p]) {
            for (idx_t q = p * p; q <= limit; q += p)
                isComposite[q] = true;
        }
    }

    /* 1 is treated as a “non‑composite” number in A175856. */
    if (count < cap) primes[count++] = 1;
    for (idx_t i = 2; i <= limit; ++i)
        if (!isComposite[i]) {
            if (count >= cap) {
                /* Should never happen for our chosen caps. */
                free(isComposite);
                return 0;
            }
            primes[count++] = i;
        }

    free(isComposite);
    return count;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined, but safe */

    /* Margin large enough for prime gaps up to the requested n. */
    const idx_t MARGIN = 5000;
    idx_t limit = n + MARGIN;

    /* Rough prime capacity estimate */
    size_t primeCap = limit / 2 + 50;
    idx_t *primeList = (idx_t *)malloc(primeCap * sizeof(idx_t));
    if (!primeList)
        return 0;                      /* allocation failure */

    size_t primeCnt = build_primes(limit, primeList, primeCap);
    if (primeCnt == 0) {
        free(primeList);
        return 0;                      /* allocation failure or overflow */
    }

    /* Frequency array for values up to n. */
    seq_t *freq = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!freq) {
        free(primeList);
        return 0;
    }

    for (size_t i = 0; i + 1 < primeCnt; ++i) {
        idx_t p = primeList[i];
        idx_t q = primeList[i + 1];
        if (q == 0) continue;          /* safety */

        idx_t d = q - p - 1;
        idx_t minV = (p > d) ? p - d : 0;   /* avoid underflow */

        idx_t start = (minV >= 1) ? minV : 1;
        idx_t end   = (p <= n)   ?   p : n;
        for (idx_t v = start; v <= end; ++v)
            freq[v] += 1;
    }

    seq_t result = freq[n];

    free(freq);
    free(primeList);
    return result;
}
