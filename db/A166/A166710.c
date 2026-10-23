/*
 * A166710  a(n)=prime(n) written in base the largest digit of prime(n).
 * https://oeis.org/A166710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the decimal representation of the nth prime written in
 *            base equal to the largest decimal digit of that prime. If the
 *            prime is a repunit (all digits are 1), the prime itself is
 *            returned
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 196, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Prime cache and generator                                         */
/* ------------------------------------------------------------------ */
static uint64_t *prime_cache = NULL;
static size_t prime_cache_cap = 0;
static size_t prime_cache_size = 0;

static void prime_cache_ensure(size_t needed)
{
    if (prime_cache_cap >= needed) return;
    size_t newcap = prime_cache_cap ? prime_cache_cap << 1 : 64;
    if (newcap < needed) newcap = needed;
    uint64_t *tmp = (uint64_t *)realloc(prime_cache, newcap * sizeof(uint64_t));
    if (!tmp) exit(1);          /* abort on allocation failure */
    prime_cache = tmp;
    prime_cache_cap = newcap;
}

static void add_prime(uint64_t p)
{
    prime_cache_ensure(prime_cache_size + 1);
    prime_cache[prime_cache_size++] = p;
}

/* Simple trial‑division prime generator using the cached primes. */
static void generate_next_prime(void)
{
    if (prime_cache_size == 0) {
        add_prime(2);
        return;
    }

    uint64_t candidate = prime_cache[prime_cache_size - 1] + 1;
    if ((candidate & 1) == 0) candidate++;   /* start with an odd number */

    for (;;) {
        bool prime = true;
        for (size_t i = 0; i < prime_cache_size; ++i) {
            uint64_t p = prime_cache[i];
            if (p > candidate / p) break;    /* p*p > candidate without overflow */
            if (candidate % p == 0) { prime = false; break; }
        }
        if (prime) {
            add_prime(candidate);
            return;
        }
        candidate += 2;                       /* next odd candidate */
    }
}

static uint64_t nth_prime(idx_t n)
{
    while (prime_cache_size < (size_t)n) generate_next_prime();
    return prime_cache[n - 1];
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    uint64_t p = nth_prime(n);          /* decimal value of the nth prime */

    /* find the largest decimal digit */
    uint64_t tmp = p;
    int max_digit = 0;
    while (tmp > 0) {
        int d = (int)(tmp % 10);
        if (d > max_digit) max_digit = d;
        tmp /= 10;
    }

    /* repunit primes – return the prime itself */
    if (max_digit == 1) return (seq_t)p;

    /* digits of p in base max_digit */
    int digits[64];                      /* enough for 64‑bit primes */
    int len = 0;
    tmp = p;
    while (tmp > 0) {
        digits[len++] = (int)(tmp % max_digit);
        tmp /= max_digit;
    }

    /* build the decimal number from the most significant digit */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        result = result * 10 + (seq_t)digits[i];
    }

    return result;
}
