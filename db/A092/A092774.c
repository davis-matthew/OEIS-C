/*
 * A092774  Prime(prime(n))^2+1.
 * https://oeis.org/A092774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(prime(n))^2 + 1
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = (p_{p_n})^2 + 1, where p_k is the k‑th prime. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* defined only for n ≥ 1 */

    /* static data for the sieve and prime list */
    static int initialized = 0;
    static idx_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_cnt = 0;
    const idx_t LIMIT = 2000000;              /* 2 million covers n≈10 000 */

    if (!initialized) {
        /* Simple sieve of Eratosthenes up to LIMIT */
        const size_t lim = (size_t)LIMIT;
        unsigned char *comp = (unsigned char*)calloc(lim + 1, 1);
        if (!comp) return 0;                 /* allocation failure */

        for (size_t i = 2; i * i <= lim; ++i) {
            if (!comp[i]) {
                for (size_t j = i * i; j <= lim; j += i)
                    comp[j] = 1;
            }
        }

        primes_cap = 128;
        primes = (idx_t*)malloc(primes_cap * sizeof(idx_t));
        if (!primes) { free(comp); return 0; }

        for (size_t i = 2; i <= lim; ++i) {
            if (!comp[i]) {
                if (primes_cnt == primes_cap) {
                    primes_cap <<= 1;               /* double the capacity */
                    idx_t *tmp = (idx_t*)realloc(primes,
                                                  primes_cap * sizeof(idx_t));
                    if (!tmp) { free(comp); free(primes); return 0; }
                    primes = tmp;
                }
                primes[primes_cnt++] = (idx_t)i;
            }
        }
        free(comp);
        initialized = 1;
    }

    /* Retrieve p_n and p_{p_n} */
    if ((size_t)n > primes_cnt) return 0;          /* n out of range */
    idx_t pn = primes[n - 1];                      /* p_n */
    if ((size_t)pn > primes_cnt) return 0;         /* not enough primes for p_{p_n} */

    idx_t ppn = primes[pn - 1];                    /* p_{p_n} */

    seq_t result = (seq_t)ppn * (seq_t)ppn + 1;    /* (p_{p_n})^2 + 1 */
    return result;
}
