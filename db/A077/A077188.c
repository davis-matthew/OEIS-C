/*
 * A077188  a(n) = A077186(n)/prime(n).
 * https://oeis.org/A077188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A077186(n)/prime(n) where A077186(n) is the square of the
 *            smallest repunit (decimal integer consisting solely of the digit
 *            1) that is divisible by the n‑th prime. For primes 2 and 5 no
 *            such repunit exists, so a(n) = 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------
   Prime table (generated lazily on first use)
   ------------------------------------------------------------ */
static bool primes_initialized = false;
static size_t primes_cnt = 0;
#define PRIME_MAX 200000          /* enough for n ≤ 10000 (≈17984 primes) */
static uint64_t primes[PRIME_MAX];

static void init_primes(void)
{
    if (primes_initialized) return;
    const unsigned int limit = PRIME_MAX;
    /* simple sieve */
    uint8_t *is_comp = (uint8_t*)calloc(limit + 1, 1);  /* 0 → prime, 1 → composite */
    for (unsigned int i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (primes_cnt < PRIME_MAX) primes[primes_cnt++] = i;
            if ((uint64_t)i * i <= limit) {
                for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                    is_comp[(size_t)j] = 1;
            }
        }
    }
    free(is_comp);
    primes_initialized = true;
}

static uint64_t nth_prime(idx_t n)
{
    /* n is 1‑based */
    if (!primes_initialized) init_primes();
    if (n == 0 || (size_t)n > primes_cnt) return 0;  /* safety */
    return primes[n - 1];
}

/* ------------------------------------------------------------
   Main sequence function
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    uint64_t p = nth_prime(n);
    if (p == 0) return 0;                 /* out of range indices */
    if (p == 2 || p == 5) return 0;       /* no repunit divisible by 2 or 5 */

    /* Find minimal k such that the k‑digit repunit R_k is divisible by p */
    uint64_t r_mod = 0;
    unsigned k = 0;
    do {
        ++k;
        r_mod = (r_mod * 10 + 1) % p;
    } while (r_mod != 0);

    /* Build the repunit R_k as a seq_t: 1, 11, 111, ... */
    seq_t rep = 0;
    for (unsigned i = 0; i < k; ++i)
        rep = rep * 10 + 1;

    /* a(n) = R_k * R_k / p = (R_k / p) * R_k (since p | R_k) */
    seq_t q = rep / (seq_t)p;
    return q * rep;
}
