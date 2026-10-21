/*
 * A097463  Let P(i) = i-th prime. To get a(n), factor P(n)-1 as a product of
 *          primes, then concatenate the exponents.
 * https://oeis.org/A097463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 22, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A097463: a(n) is the concatenation of the prime‑factor exponents of P(n)-1,
 * where P(n) is the nth prime.  For n>=2 we write the exponents of all
 * primes up to the largest prime factor (including zeros for missing factors)
 * in ascending order and concatenate them as decimal digits.  a(1)=0. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Prime storage and accessor ---------- */
static uint64_t *primes          = NULL;   /* array of primes */
static size_t   primes_size     = 0;      /* number of primes stored */
static size_t   primes_capacity = 0;      /* allocated capacity */

/* Return the nth prime (1‑based).  Generates primes lazily as needed. */
static uint64_t nth_prime(idx_t n)
{
    /* Ensure the primes array has at least n elements. */
    if (primes_size < n) {
        size_t cap = primes_capacity;
        if (cap == 0) {
            cap = 16;
            primes = (uint64_t*)malloc(cap * sizeof(uint64_t));
            if (!primes) exit(1);
            primes_capacity = cap;
        }
        uint64_t candidate = (primes_size == 0) ? 2 : primes[primes_size - 1] + 1;
        while (primes_size < n) {
            bool is_p = true;
            for (size_t i = 0; i < primes_size && primes[i] * primes[i] <= candidate; ++i) {
                if (candidate % primes[i] == 0) { is_p = false; break; }
            }
            if (is_p) {
                if (primes_size == primes_capacity) {
                    primes_capacity *= 2;
                    primes = (uint64_t*)realloc(primes,
                                                primes_capacity * sizeof(uint64_t));
                    if (!primes) exit(1);
                }
                primes[primes_size++] = candidate;
            }
            ++candidate;
        }
    }
    return primes[n - 1];
}

/* ---------- Helper: append decimal digits of an integer to a result ---------- */
static void append_digits(seq_t *res, int e)
{
    char buf[32];
    int len = 0;
    if (e == 0) {
        len = 1;
        buf[0] = '0';
    } else {
        while (e > 0) {
            buf[len++] = '0' + (e % 10);
            e /= 10;
        }
    }
    for (int i = len - 1; i >= 0; --i) {
        *res = (*res) * 10 + (seq_t)(buf[i] - '0');
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;

    uint64_t p = nth_prime(n);      /* nth prime */
    uint64_t v = p - 1;             /* P(n)-1 to factor */

    /* Factor v: store distinct prime factors and their exponents. */
    uint64_t facp[64];
    int      fex [64];
    size_t   fac_sz = 0;

    uint64_t temp = v;
    for (size_t i = 0; i < primes_size && primes[i] <= temp; ++i) {
        uint64_t q = primes[i];
        if (q * q > temp) break;
        if (temp % q == 0) {
            int cnt = 0;
            while (temp % q == 0) {
                temp /= q;
                ++cnt;
            }
            facp[fac_sz] = q;
            fex [fac_sz] = cnt;
            ++fac_sz;
        }
    }
    if (temp > 1) { /* remaining prime factor */
        facp[fac_sz] = temp;
        fex [fac_sz] = 1;
        ++fac_sz;
    }

    /* Compute concatenation of exponents over primes up to the largest factor. */
    uint64_t maxp = (fac_sz > 0) ? facp[fac_sz - 1] : 0;
    seq_t result = 0;
    size_t fac_idx = 0;  /* pointer into facp/fex arrays */

    for (size_t pi = 0; pi < primes_size && primes[pi] <= maxp; ++pi) {
        int exp = 0;
        if (fac_idx < fac_sz && facp[fac_idx] == primes[pi]) {
            exp = fex[fac_idx];
            ++fac_idx;
        }
        append_digits(&result, exp);
    }
    return result;
}
