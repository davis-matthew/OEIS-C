/*
 * A073342  Average digit (rounded to the nearest integer) in the decimal
 *          expansion of n-th prime.
 * https://oeis.org/A073342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = round( digit_sum(p_n) / digit_count(p_n) )
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: average digit (rounded to nearest integer) in the decimal expansion of the n‑th prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cache of primes discovered so far. */
static uint64_t *primes = NULL;
static size_t primes_count = 0;
static size_t primes_cap = 0;
static uint64_t candidate = 5;            /* next odd to test after 3 */

/* Integer floor square root: returns ⌊√x⌋. */
static uint64_t int_sqrt(uint64_t x)
{
    uint64_t r = 0, bit = 1ULL << 62;  /* largest power of four <= 2^64 */
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Ensure at least n primes are in the cache. (n ≥ 1) */
static void ensure_primes(idx_t n)
{
    if (primes_count >= (size_t)n)
        return;

    if (primes_count == 0) {                    /* first call */
        primes_cap = 4;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes[0] = 2;
        primes[1] = 3;
        primes_count = 2;
        candidate = 5;
    }

    while (primes_count < (size_t)n) {
        uint64_t limit = int_sqrt(candidate);
        bool is_prime = true;
        for (size_t i = 0; i < primes_count && primes[i] <= limit; ++i) {
            if (candidate % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_count == primes_cap) {
                primes_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) abort();      /* allocation failed */
                primes = tmp;
            }
            primes[primes_count++] = candidate;
        }
        candidate += 2;                      /* test next odd number */
    }
}

/* Sum of decimal digits of a uint64_t. */
static uint64_t digit_sum_uint64(uint64_t v)
{
    uint64_t s = 0;
    while (v) {
        s += v % 10;
        v /= 10;
    }
    return s;
}

/* Count of decimal digits of a uint64_t. */
static uint64_t digit_count_uint64(uint64_t v)
{
    uint64_t c = 0;
    do {
        ++c;
        v /= 10;
    } while (v);
    return c;
}

/* OEIS sequence A073342. */
seq_t a(idx_t n)
{
    if (n == 0)                      /* undefined by the OEIS offset; defensive return */
        return (seq_t)0;

    ensure_primes(n);

    uint64_t p = primes[(size_t)(n - 1)];     /* n‑th prime (1‑based) */

    uint64_t sum  = digit_sum_uint64(p);
    uint64_t count = digit_count_uint64(p);

    /* Rounded nearest integer: floor((sum*2 + count) / (2*count)). */
    seq_t result = (seq_t)((sum * 2 + count) / (2 * count));

    return result;
}
