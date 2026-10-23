/*
 * A107303  Numbers k such that (3*k - 5) is prime.
 * https://oeis.org/A107303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 8847, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that 3*k‑5 is prime.  
   Computed by generating primes p ≡ 1 (mod 3) and storing k = (p+5)/3. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Deterministic Miller–Rabin for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;

    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i])
            return true;
        if (n % small_primes[i] == 0)
            return false;
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0ULL)
            continue;

        __uint128_t x = 1, power = a;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1ULL)
                x = x * power % n;
            power = power * power % n;
            exp >>= 1;
        }
        uint64_t x64 = (uint64_t)x;
        if (x64 == 1ULL || x64 == n - 1)
            continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x64 = (__uint128_t)x64 * x64 % n;
            if (x64 == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* Global memoisation storage */
static seq_t *seq_table = NULL;
static idx_t seq_size = 0;   /* number of terms stored */
static idx_t seq_cap   = 0;   /* allocated capacity */

static void seq_alloc(size_t new_cap)
{
    seq_t *new_table = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_table)
        exit(EXIT_FAILURE);
    if (seq_table) {
        memcpy(new_table, seq_table, seq_size * sizeof(seq_t));
        free(seq_table);
    }
    seq_table = new_table;
    seq_cap   = (idx_t)new_cap;
}

static void ensure_terms(idx_t n)
{
    if (n == 0)
        return;

    if (!seq_cap)
        seq_alloc(16);

    static uint64_t prime_candidate = 7;   /* start with the first prime ≡1 mod 3 */

    while (seq_size < n) {
        if (seq_size == seq_cap)
            seq_alloc(seq_cap << 1);

        /* Find next prime ≡1 mod 3 */
        while (!(prime_candidate % 3ULL == 1ULL && is_prime_uint64(prime_candidate)))
            ++prime_candidate;

        /* Store k = (p+5)/3 */
        uint64_t k = (prime_candidate + 5ULL) / 3ULL;
        seq_table[seq_size] = (seq_t)k;
        ++seq_size;

        ++prime_candidate;   /* advance for the next search */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)   /* guard against out‑of‑range call */
        return (seq_t)0;
    ensure_terms(n);
    return seq_table[n - 1];
}
