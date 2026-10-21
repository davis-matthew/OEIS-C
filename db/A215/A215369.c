/*
 * A215369  Numbers n with the same f-value as n+1, where f(n) = A056239(n) =
 *          Sum_{i} i*e_i for n = Product_{i} prime(i)^e_i.
 * https://oeis.org/A215369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 180, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A215369: Numbers n such that f(n) = f(n+1),
   where f(n) = Σ i·e_i for n = Π prime(i)^e_i.  Uses a simple
   sieve and trial division, storing found terms in a static array
   so arbitrary order of calls is supported. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime sieve: primes up to LIMIT ---------- */
static const uint32_t LIMIT = 2000000;   /* should cover all primes needed */
static uint32_t *prime_arr = NULL;       /* array of primes */
static size_t prime_count = 0;           /* number of primes */
static bool primes_inited = false;

/* Allocate and fill prime_arr once */
static void init_primes(void)
{
    uint8_t *sieve = (uint8_t*)calloc(LIMIT + 1, 1);
    sieve[0] = sieve[1] = 1;
    for (uint32_t i = 2; i * i <= LIMIT; ++i)
        if (!sieve[i])
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = 1;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i])
            ++prime_count;
    prime_arr = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    size_t idx = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i])
            prime_arr[idx++] = i;
    free(sieve);
}

/* Binary search of prime rank (0‑based) in prime_arr */
static size_t find_prime_index(uint32_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_arr[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;   /* prime_arr[lo] == p */
}

/* Compute f(n) as defined by A056239 */
static seq_t compute_f(uint64_t n)
{
    seq_t sum = 0;
    uint64_t temp = n;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_arr[i];
        if ((uint64_t)p * p > temp)
            break;
        if (temp % p == 0) {
            uint32_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            sum += ((seq_t)(i + 1)) * exp;
        }
    }
    if (temp > 1) {                     /* remaining prime factor */
        size_t idx = find_prime_index((uint32_t)temp);
        sum += (seq_t)(idx + 1);
    }
    return sum;
}

/* ---------- Sequence storage ---------- */
static seq_t *seq_vals = NULL;   /* array of found terms */
static size_t seq_len = 0;       /* number of terms stored */
static size_t seq_cap = 0;       /* capacity of seq_vals */
static uint64_t last_checked = 1;   /* next integer to test */

/* Ensure that at least 'required' terms are stored */
static void ensure_sequence_upto(idx_t required)
{
    if (seq_len >= required)
        return;

    if (!seq_vals) {
        seq_cap = 16;
        seq_vals = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    }

    while (seq_len < required) {
        /* Find next term n with f(n) == f(n+1) */
        while (true) {
            seq_t f1 = compute_f(last_checked);
            seq_t f2 = compute_f(last_checked + 1);
            if (f1 == f2)
                break;
            ++last_checked;
        }
        /* Store the term */
        if (seq_len == seq_cap) {
            seq_cap *= 2;
            seq_vals = (seq_t*)realloc(seq_vals, seq_cap * sizeof(seq_t));
        }
        seq_vals[seq_len++] = (seq_t)last_checked;
        ++last_checked;
    }
}

/* ---------- Public API ---------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* undefined for n=0 in OEIS context */

    if (!primes_inited) {
        init_primes();
        primes_inited = true;
    }

    ensure_sequence_upto(n);
    return seq_vals[n - 1];
}
