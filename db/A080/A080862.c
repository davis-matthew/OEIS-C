/*
 * A080862  Numbers which can be expressed as the sum of two distinct primes
 *          in exactly six ways.
 * https://oeis.org/A080862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer that can be expressed as the *
 *            sum of two distinct primes in exactly six distinct ways. * (a(1)
 *            = 60, a(2) = 66, …, a(19) = 398; there are no further terms.)
 * Range:     u8 n <= 15, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* The build will provide seq_t and idx_t types.  seq_t is an integer
 * type that may be 8, 16, 32, 64 bits wide or an arbitrary‑precision type.
 */
seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------
     *  Static data that is lazily initialised once.
     * ------------------------------------------------------------------ */
    static bool        initialized = false;
    static seq_t      *terms       = NULL;
    static size_t      terms_len   = 0;
    static size_t      terms_cap   = 0;

    /* ------------------------------------------------------------------
     *  Parameters for the search space.  The OEIS entry states that
     *  there are no further terms beyond 398.
     * ------------------------------------------------------------------ */
    const size_t SEARCH_LIMIT = 50000;          /* ample room for all terms */
    const size_t PRIME_LIMIT  = SEARCH_LIMIT;  /* sieve upper bound      */
    const size_t INITIAL_CAP  = 32;             /* initial storage size   */

    /* ------------------------------------------------------------------
     *  Initialise primes and the list of terms on the first call.
     * ------------------------------------------------------------------ */
    if (!initialized) {
        /* --- 1. Sieve of Eratosthenes up to PRIME_LIMIT --- */
        unsigned char *prime_flag = (unsigned char *)malloc((PRIME_LIMIT + 1) * sizeof(unsigned char));
        if (prime_flag == NULL) return 0;
        for (size_t i = 0; i <= PRIME_LIMIT; ++i) prime_flag[i] = 1;
        prime_flag[0] = prime_flag[1] = 0;
        for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
            if (prime_flag[i]) {
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i) prime_flag[j] = 0;
            }
        }

        /* --- 2. Collect the primes into an array --- */
        size_t prime_cnt = 0;
        for (size_t i = 2; i <= PRIME_LIMIT; ++i)
            if (prime_flag[i]) ++prime_cnt;
        uint32_t *primes = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
        if (primes == NULL) {
            free(prime_flag);
            return 0;
        }
        size_t idx = 0;
        for (size_t i = 2; i <= PRIME_LIMIT; ++i)
            if (prime_flag[i]) primes[idx++] = (uint32_t)i;

        /* --- 3. Search for all numbers with exactly six representations --- */
        terms_cap = INITIAL_CAP;
        terms     = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (terms == NULL) {
            free(prime_flag);
            free(primes);
            return 0;
        }
        terms_len = 0;

        for (size_t val = 4; val <= SEARCH_LIMIT; ++val) {
            int reps = 0;
            for (size_t pi = 0; pi < prime_cnt; ++pi) {
                uint32_t p = primes[pi];
                if (p * 2 >= val) break;                /* no larger pairs possible */
                uint32_t q = (uint32_t)(val - p);
                if (q <= p) continue;                  /* enforce distinctness, p<q */
                if (q <= PRIME_LIMIT && prime_flag[q]) reps++;
                else if (q > PRIME_LIMIT) {
                    /* For q beyond the sieve, perform a simple primality test.
                       This case never occurs for the known terms, but is kept
                       for correctness for arbitrary limits.                    */
                    bool q_is_prime = true;
                    for (uint32_t d = 2; (uint64_t)d * d <= q; ++d)
                        if (q % d == 0) { q_is_prime = false; break; }
                    if (q_is_prime) reps++;
                }
            }
            if (reps == 6) {
                if (terms_len == terms_cap) {
                    terms_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                    if (tmp == NULL) {
                        free(prime_flag);
                        free(primes);
                        free(terms);
                        return 0;
                    }
                    terms = tmp;
                }
                terms[terms_len++] = (seq_t)val;
            }
        }

        /* --- 4. Clean up temporary structures --- */
        free(prime_flag);
        free(primes);

        initialized = true;
    }

    /* ------------------------------------------------------------------
     *  Return the requested term; return 0 for out of range or bad indices.
     * ------------------------------------------------------------------ */
    if (n == 0) return 0;                         /* sequence is 1‑indexed */
    if ((size_t)n <= terms_len) return terms[n - 1];
    return 0;                                     /* undefined term */
}
