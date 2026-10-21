/*
 * A074770  Numbers n such that tau(n) > tau(n+1), phi(n) > phi(n+1) and
 *          sigma(n) > sigma(n+1).
 * https://oeis.org/A074770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 206, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A074770 – numbers n such that:
   tau(n) > tau(n+1), phi(n) > phi(n+1) and sigma(n) > sigma(n+1).
   The sequence is generated on demand by testing successive integers
   and caching all terms found so far.  For each candidate m we
   factorise m and m+1 on the fly (trial division using a pre‑computed
   list of primes) and compute τ, φ and σ in type seq_t.  All
   intermediate arithmetic uses only + - * / % and comparisons, so
   the code is valid for both the C and C++ build environments.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- 1. Static storage for the generated sequence ---------- */
static seq_t *seq_arr = NULL;   /* dynamic array of terms */
static size_t seq_len = 0;      /* number of stored terms   */
static size_t seq_cap = 0;      /* current array capacity  */
static idx_t  last_m  = 0;      /* last integer examined   */

/* ---------- 2. Prime table (simple sieve, up to 2 000 000) ---------- */
#define PRIME_CAP 2000000  /* enough for all candidates needed for n≈10 000 */
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    static bool sieve[PRIME_CAP + 1];
    size_t i, j;

    /* simple Sieve of Eratosthenes */
    for (i = 0; i <= PRIME_CAP; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (i = 2; i * i <= PRIME_CAP; ++i)
        if (sieve[i])
            for (j = i * i; j <= PRIME_CAP; j += i)
                sieve[j] = false;

    primes = (uint32_t *)malloc((PRIME_CAP / 10) * sizeof(uint32_t));
    primes_cnt = 0;
    for (i = 2; i <= PRIME_CAP; ++i)
        if (sieve[i])
            primes[primes_cnt++] = (uint32_t)i;
    primes_inited = true;
}

/* ---------- 3. helpers for manipulating seq_t values ---------- */
static seq_t seq_pow_uint64(seq_t base, uint64_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1ULL) result *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return result;
}

/* Compute tau, phi and sigma of m, returning them via pointers.  All
   calculations are performed in type seq_t. */
static void factor_stats(idx_t m, seq_t *tau, seq_t *phi, seq_t *sigma)
{
    uint64_t n = (uint64_t)m;
    seq_t tau_val = (seq_t)1;
    seq_t phi_val = (seq_t)m;
    seq_t sigma_val = (seq_t)1;

    uint64_t temp = n;
    for (size_t pi = 0; pi < primes_cnt && (uint64_t)primes[pi] * (uint64_t)primes[pi] <= temp; ++pi) {
        uint32_t p = primes[pi];
        if (temp % p != 0) continue;
        uint64_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        tau_val *= (seq_t)(exp + 1);
        phi_val /= (seq_t)p;
        phi_val *= (seq_t)(p - 1);

        /* sigma contribution: (p^(exp+1)-1)/(p-1) */
        seq_t numerator = seq_pow_uint64((seq_t)p, exp + 1) - (seq_t)1;
        seq_t denominator = (seq_t)(p - 1);
        sigma_val *= numerator / denominator;
    }
    if (temp > 1) { /* remaining prime factor */
        uint64_t p = temp;
        tau_val *= (seq_t)2;
        phi_val /= (seq_t)p;
        phi_val *= (seq_t)(p - 1);
        sigma_val *= (seq_t)(1 + p);
    }

    *tau = tau_val;
    *phi = phi_val;
    *sigma = sigma_val;
}

/* ---------- 4. Store next term in the sequence ---------- */
static void push_term(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    seq_arr[seq_len++] = val;
}

/* ---------- 5. Main public interface ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1 */

    init_primes();     /* ensure prime list is ready */

    while (seq_len < n) {          /* generate until we have n terms */
        ++last_m;                  /* next candidate value m */

        seq_t tau_m, phi_m, sigma_m;
        seq_t tau_m1, phi_m1, sigma_m1;

        factor_stats((idx_t)last_m,   &tau_m,   &phi_m,   &sigma_m);
        factor_stats((idx_t)(last_m + 1), &tau_m1, &phi_m1, &sigma_m1);

        if (tau_m <= tau_m1)       continue;
        if (phi_m <= phi_m1)       continue;
        if (sigma_m <= sigma_m1)   continue;

        /* All conditions satisfied – record the term. */
        push_term((seq_t)last_m);
    }

    return seq_arr[n - 1];
}
