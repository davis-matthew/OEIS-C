/*
 * A059591  Squarefree part of n^2+1.
 * https://oeis.org/A059591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A007913(n^2+1) – squarefree part of n^2+1
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Prime sieve limit sufficient for n up to ~10⁴ (n²+1 < 10⁸). */
#define PRIME_LIMIT 20000

static int primes[PRIME_LIMIT];
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void build_primes(void)
{
    if (primes_ready) return;
    bool marked[PRIME_LIMIT + 1];
    for (int i = 0; i <= PRIME_LIMIT; ++i) marked[i] = false;
    for (int i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!marked[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i) marked[j] = true;
        }
    }
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (!marked[i]) primes[prime_cnt++] = i;
    }
    primes_ready = true;
}

/* Return the squarefree part of n^2 + 1. */
seq_t a(idx_t n)
{
    build_primes();

    if (n == 0) return (seq_t)1;

    __int128 val  = (__int128)n * n + 1;
    __int128 temp = val;
    seq_t res = 1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        int p = primes[i];
        if ((__int128)p * p > temp) break;

        int parity = 0;
        while (temp % p == 0) {
            temp /= p;
            parity ^= 1;              /* toggle parity */
        }
        if (parity) {
            /* Multiplication in a wider type to avoid overflow during computation. */
            res = (seq_t)((unsigned long long)res * (unsigned long long)p);
        }
    }

    if (temp > 1) {
        res = (seq_t)((unsigned long long)res * (unsigned long long)temp);
    }

    return res;
}
