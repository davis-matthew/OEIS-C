/*
 * A062284  Primes p such that p + 50 is also prime.
 * https://oeis.org/A062284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p for which p + 50 is also prime (list in
 *            increasing order)
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset: 1 → first term */

    /* static tables initialised once */
    static bool init = false;
    static idx_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t *pair_primes = NULL;
    static size_t pair_cnt = 0;
    const idx_t LIMIT = 2000000;     /* high enough for many thousands of terms */

    if (!init) {
        /* ---------- sieve of Eratosthenes ---------- */
        size_t sz = (size_t)LIMIT + 1;
        unsigned char *isprime = (unsigned char *)malloc(sz);
        if (!isprime) return 0;
        for (size_t i = 0; i < sz; ++i) isprime[i] = 1;
        isprime[0] = isprime[1] = 0;

        for (idx_t p = 2; p * p <= LIMIT; ++p) {
            if (isprime[p]) {
                for (idx_t q = p * p; q <= LIMIT; q += p)
                    isprime[q] = 0;
            }
        }

        /* ---------- gather all primes up to LIMIT ---------- */
        size_t cap = 65536;
        primes = (idx_t *)malloc(cap * sizeof(idx_t));
        if (!primes) { free(isprime); return 0; }
        for (idx_t i = 2; i <= LIMIT; ++i) {
            if (isprime[i]) {
                if (primes_cnt == cap) {
                    cap <<= 1;
                    primes = (idx_t *)realloc(primes, cap * sizeof(idx_t));
                }
                primes[primes_cnt++] = i;
            }
        }

        /* ---------- select primes whose successor at +50 is prime ---------- */
        size_t pcap = 128;
        pair_primes = (idx_t *)malloc(pcap * sizeof(idx_t));
        if (!pair_primes) { free(isprime); free(primes); return 0; }
        for (size_t i = 0; i < primes_cnt; ++i) {
            idx_t p = primes[i];
            if (p + 50 <= LIMIT && isprime[p + 50]) {
                if (pair_cnt == pcap) {
                    pcap <<= 1;
                    pair_primes = (idx_t *)realloc(pair_primes, pcap * sizeof(idx_t));
                }
                pair_primes[pair_cnt++] = p;
            }
        }

        free(isprime);
        init = true;
    }

    /* ---------- answer query ---------- */
    if ((size_t)(n - 1) >= pair_cnt) return 0;  /* out of bounds, should not happen */
    return (seq_t)pair_primes[n - 1];
}
