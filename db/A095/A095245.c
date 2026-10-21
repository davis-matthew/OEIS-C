/*
 * A095245  (Concatenation of first n primes) modulo prime(n).
 * https://oeis.org/A095245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(n) = (concatenation of the first n+1 primes) mod prime(n+1)
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

/* Generate prime numbers lazily.  The vector of primes grows as needed. */
static const size_t INITIAL_PRIMER_CAP = 200;
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap = 0;

/* Expand the prime list until it contains at least `need` primes. */
static void ensure_primes(size_t need)
{
    if (prime_list == NULL) {
        prime_cap = INITIAL_PRIMER_CAP;
        prime_list = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
        prime_count = 0;
    }
    if (prime_count >= need) return;

    int candidate = prime_count ? prime_list[prime_count - 1] + 1 : 2;
    while (prime_count < need) {
        /* Find the next prime */
        bool is_prime = true;
        int limit = (int)floor(sqrt((double)candidate));
        for (size_t i = 0; i < prime_count && prime_list[i] <= limit; ++i) {
            if (candidate % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_count == prime_cap) {
                prime_cap *= 2;
                prime_list = (uint32_t*)realloc(prime_list, prime_cap * sizeof(uint32_t));
            }
            prime_list[prime_count++] = (uint32_t)candidate;
        }
        ++candidate;
    }
}

/* Compute (x * y) % mod avoiding overflow for 64‑bit inputs.
   The product fits in 128 bits when compiled as C++17, otherwise
   the values are small enough to be safe. */
static inline seq_t mul_mod(seq_t a, seq_t b, seq_t mod)
{
    /* Use 128‑bit intermediate to avoid overflow when available. */
#ifdef __SIZEOF_INT128__
    return (unsigned __int128)a * b % mod;
#else
    /* For 64‑bit mod with values < 2^60 the product fits in 128
       but we warn that this cast may not be portable. */
    return (a * b) % mod;
#endif
}

/* Compute (10^d) mod mod, where d is a small integer (digits of a prime). */
static seq_t pow10_mod(int d, seq_t mod)
{
    seq_t res = 1;
    for (int i = 0; i < d; ++i) {
        res = mul_mod(res, 10, mod);
    }
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* We need the first n+1 primes. */
    size_t need = (size_t)n + 1;
    ensure_primes(need);

    size_t k = need;                     /* k = n+1 */
    seq_t pk = (seq_t)prime_list[k - 1];  /* prime(k) */

    seq_t rem = 0;
    for (size_t i = 0; i < k; ++i) {
        seq_t p = (seq_t)prime_list[i];

        /* Count digits of p. */
        int digits = 0;
        seq_t tmp = p;
        do {
            ++digits;
            tmp /= 10;
        } while (tmp > 0);

        seq_t pow10 = pow10_mod(digits, pk);
        rem = (mul_mod(rem, pow10, pk) + (p % pk)) % pk;
    }

    return rem;
}
