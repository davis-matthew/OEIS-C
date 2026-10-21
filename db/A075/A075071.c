/*
 * A075071  n! divided by product of factorials of all proper divisors of n,
 *          as n runs through the values for which the result is an integer.
 * https://oeis.org/A075071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = n! / ∏_{d|n, d!=n} d! (return 0 if not an integer)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 12, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* ---------- static data --------------------------------------------- */
#define MAX_PRIME_CAP 20000
static uint64_t primes[MAX_PRIME_CAP];
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* ---------- helper: build primes up to 'limit' ---------------------- */
static void build_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* allocate sieve */
    unsigned char *sieve = (unsigned char *)malloc((size_t)(limit + 1));
    if (!sieve)
        return;                      /* unlikely */

    for (size_t i = 0; i <= (size_t)limit; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (size_t p = 2; p * p <= limit; ++p) {
        if (sieve[p]) {
            for (size_t m = p * p; m <= (size_t)limit; m += p)
                sieve[m] = 0;
        }
    }

    /* collect primes */
    prime_count = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (sieve[i]) {
            if (prime_count < MAX_PRIME_CAP)
                primes[prime_count++] = i;
        }
    }

    free(sieve);
    prime_limit = limit;
}

/* ---------- main sequence function ---------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1, undefined for 0 */

    /* make sure we have primes up to n */
    if (prime_limit < n)
        build_primes(n);

    /* exponent array for the factorisation of the answer */
    static int exp_arr[MAX_PRIME_CAP];
    for (size_t i = 0; i < prime_count; ++i)
        exp_arr[i] = 0;

    /* contributions of n! (Legendre's formula) */
    for (size_t pi = 0; pi < prime_count; ++pi) {
        idx_t p = primes[pi];
        if (p > n)
            break;
        idx_t t = n;
        int e = 0;
        while (t) {
            t /= p;
            e += (int)t;
        }
        exp_arr[pi] = e;
    }

    /* enumerate proper divisors and subtract their factorial parts */
    static idx_t divs[512];
    size_t divcnt = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d, d2 = n / d;
            if (d1 != n && divcnt < 512)
                divs[divcnt++] = d1;
            if (d2 != n && d2 != d1 && divcnt < 512)
                divs[divcnt++] = d2;
        }
    }

    for (size_t vi = 0; vi < divcnt; ++vi) {
        idx_t d = divs[vi];
        for (size_t pi = 0; pi < prime_count; ++pi) {
            idx_t p = primes[pi];
            if (p > d)
                break;
            idx_t t = d;
            int e = 0;
            while (t) {
                t /= p;
                e += (int)t;
            }
            exp_arr[pi] -= e;
            if (exp_arr[pi] < 0)
                return (seq_t)0;   /* not an integer */
        }
    }

    /* build the final integer from prime exponents */
    seq_t result = 1;
    for (size_t pi = 0; pi < prime_count; ++pi) {
        int e = exp_arr[pi];
        if (e > 0) {
            seq_t p_pow = seq_pow((seq_t)primes[pi], (idx_t)e);
            result = seq_mul(result, p_pow);
        }
    }

    return result;
}
