/*
 * A106110  Primes having only {7, 8, 9} as digits.
 * https://oeis.org/A106110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 2570, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A106110: a(n) = nth prime that consists solely of the decimal digits 7, 8 and 9. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Deterministic Miller–Rabin for 64‑bit integers.  Works for all n < 2^64. */
static bool isPrime64(uint64_t n)
{
    if (n < 2)               return false;
    if (n % 2 == 0)          return n == 2;
    if (n % 3 == 0)          return n == 3;

    /* Write n-1 as d * 2^s */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* Deterministic bases for 64‑bit integers. */
    const uint64_t bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    const int nb = sizeof(bases) / sizeof(bases[0]);

    for (int i = 0; i < nb; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        /* Compute a^d mod n */
        __uint128_t x = 1, p = a;
        uint64_t e = d;
        while (e) {
            if (e & 1) x = (x * p) % n;
            p = (p * p) % n;
            e >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static cache of primes found so far */
    static seq_t *primes = NULL;
    static idx_t primes_cnt = 0;
    static idx_t primes_cap = 0;

    /* current candidate in digit form */
    static int cand_len = 0;
    static char cand[32];
    static bool initialized = false;

    if (!initialized) {
        /* start with the smallest 1‑digit candidate, 7 */
        cand[0] = '7';
        cand_len = 1;
        primes_cap = 8192;
        primes = (seq_t *)malloc(sizeof(seq_t) * primes_cap);
        primes_cnt = 0;
        initialized = true;
    }

    /* Generate candidates until we have at least n primes */
    while (primes_cnt < n) {
        /* convert the current candidate to a 64‑bit integer */
        uint64_t val = 0;
        for (int i = 0; i < cand_len; ++i) {
            val = val * 10 + (uint64_t)(cand[i] - '0');
        }

        /* test for primality */
        if (isPrime64(val)) {
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (seq_t *)realloc(primes, sizeof(seq_t) * primes_cap);
            }
            primes[primes_cnt] = (seq_t)val;
            primes_cnt++;
        }

        /* advance to the next allowed number (base‑3 over {7,8,9}) */
        int i = cand_len - 1;
        for (; i >= 0; --i) {
            if (cand[i] == '7') { cand[i] = '8'; break; }
            if (cand[i] == '8') { cand[i] = '9'; break; }
            cand[i] = '7';  /* wrap from 9 to 7 and carry */
        }
        if (i < 0) { /* all digits were '9' → new, longer number */
            cand_len++;
            for (int j = 0; j < cand_len; ++j) cand[j] = '7';
        }
    }

    return primes[n - 1];
}
