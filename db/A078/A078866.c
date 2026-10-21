/*
 * A078866  The quadruples (d1,d2,d3,d4) with elements in {2,4,6} are listed
 *          in lexicographic order; for each quadruple, this sequence lists
 *          the smallest prime p >= 5 such that the differences between the 5
 *          consecutive primes starting with p are (d1,d2,d3,d4), if such a
 *          prime exists.
 * https://oeis.org/A078866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = smallest prime p ≥ 5 such that the 5 consecutive primes *
 *            starting at p have successive gaps (d1,d2,d3,d4) where *
 *            (d1,d2,d3,d4) runs through all 4‑tuples with elements in {2,4,6}
 *            in * lexicographic order; only the tuples for which such a prime
 *            exists * contribute a term. The sequence ther...
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static seq_t seq[26] = {0};
static bool initialized = false;

static void compute_sequence(void)
{
    if (initialized) return;

    /* Upper bound for prime sieve.  5×10^7 primes are plenty for the
     * 26 solutions.  The sieve uses a bitset in a byte array. */
    const uint64_t LIMIT = 50000000ULL;
    unsigned char *comp = (unsigned char *)malloc(LIMIT + 1);
    if (!comp) return;                        /* allocation failure */
    memset(comp, 0, LIMIT + 1);

    /* Dynamic array for primes */
    size_t prime_cap = 1024;
    uint64_t *primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    if (!primes) { free(comp); return; }
    size_t prime_cnt = 0;

    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (!comp[i]) {
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
                if (!tmp) { free(primes); free(comp); return; }
                primes = tmp;
            }
            primes[prime_cnt++] = i;
            if (i * i <= LIMIT) {
                for (uint64_t j = i * i; j <= LIMIT; j += i)
                    comp[j] = 1;
            }
        }
    }
    free(comp);   /* sieve finished, no longer needed */

    const unsigned dvals[3] = {2, 4, 6};
    size_t seqcnt = 0;

    for (size_t i1 = 0; i1 < 3; ++i1) {
        unsigned d1 = dvals[i1];
        for (size_t i2 = 0; i2 < 3; ++i2) {
            unsigned d2 = dvals[i2];
            for (size_t i3 = 0; i3 < 3; ++i3) {
                unsigned d3 = dvals[i3];
                for (size_t i4 = 0; i4 < 3; ++i4) {
                    unsigned d4 = dvals[i4];

                    uint64_t found = 0;
                    for (size_t ip = 0; ip + 4 < prime_cnt; ++ip) {
                        uint64_t p = primes[ip];
                        if (p < 5) continue;
                        if (primes[ip + 1] - p != d1) continue;
                        if (primes[ip + 2] - primes[ip + 1] != d2) continue;
                        if (primes[ip + 3] - primes[ip + 2] != d3) continue;
                        if (primes[ip + 4] - primes[ip + 3] != d4) continue;
                        found = p;
                        break;
                    }
                    if (found) {
                        seq[seqcnt++] = (seq_t)found;
                        if (seqcnt == 26)  /* all solutions found */ {
                            /* nothing else to do */
                        }
                    }
                }
            }
        }
    }

    free(primes);
    initialized = true;
}

seq_t a(idx_t n)
{
    compute_sequence();
    if (n == 0) return (seq_t)0;
    idx_t pos = n - 1;
    if (pos >= 26) return (seq_t)0;  /* no further terms defined */
    return seq[pos];
}
