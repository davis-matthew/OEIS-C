/*
 * A084715  Smallest number of the form n*k + 1 that is divisible by all the
 *          phi(n) numbers less than n and relatively prime to n.
 * https://oeis.org/A084715
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   modinv_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A084715
 *  a(n) = smallest number of the form n*k+1 that is divisible by all
 *        m < n with gcd(m,n)=1.  Implementation uses the formula:
 *        let L = lcm{ m | 1 ≤ m < n , gcd(m,n)=1 }.  Then
 *        a(n) = L * inv_mod( L mod n , n ), with the adjustment
 *        that if the result equals 1 (which happens for n=2) we add
 *        L*n to force k ≥ 1.
 *
 *  The algorithm computes L as a product over primes p ∤ n of
 *  p^{ ⌊log_p(n-1)⌋ }, which is the lcm of all numbers coprime to n.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/modinv_int.h"

/* ---------- Utility functions ---------- */

static void build_primes_up_to(uint32_t limit, uint32_t **out, size_t *cnt) {
    uint32_t *pr = NULL;
    size_t c = 0;
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) { *out = NULL; *cnt = 0; return; }
    for (uint32_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;
    for (uint32_t p = 2; (uint64_t)p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (uint32_t j = p * p; j <= limit; j += p) is_prime[j] = false;
        }
    }
    for (uint32_t i = 2; i <= limit; ++i) if (is_prime[i]) ++c;
    pr = (uint32_t *)malloc(c * sizeof(uint32_t));
    size_t idx = 0;
    for (uint32_t i = 2; i <= limit; ++i) if (is_prime[i]) pr[idx++] = i;
    free(is_prime);
    *out = pr;
    *cnt = c;
}

/* ---------- Global prime table ---------- */

static uint32_t *global_primes = NULL;
static size_t global_prime_cnt = 0;
static uint32_t global_prime_limit = 0;

static void ensure_prime_table(uint32_t up_to) {
    if (global_prime_limit >= up_to) return;
    /* free previous */
    free(global_primes);
    build_primes_up_to(up_to, &global_primes, &global_prime_cnt);
    global_prime_limit = up_to;
}

/* ---------- Main sequence function ---------- */

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;   /* undefined below offset */

    /* Ensure primes up to n-1 are available */
    if (n > 1) ensure_prime_table((uint32_t)n - 1);

    seq_t L = (seq_t)1;   /* cumulative lcm */
    /* Compute L as product of prime powers not dividing n */
    for (size_t i = 0; i < global_prime_cnt; ++i) {
        uint32_t p = global_primes[i];
        if (p >= n) break;
        if (n % p == 0) continue;         /* prime divides n -> skip */
        seq_t p_pow = (seq_t)1;
        while (p_pow * (seq_t)p < (seq_t)n)
            p_pow *= (seq_t)p;
        L *= p_pow;
    }

    /* Compute modular inverse of L mod n */
    int L_mod_n = (int)(L % (seq_t)n);
    int inv = modinv_int(L_mod_n, (int)n);

    /* Final value */
    seq_t x = L * (seq_t)inv;
    if (x == (seq_t)1) {
        /* this only happens for n == 2, need k >= 1 */
        x += L * (seq_t)n;
    }
    return x;
}
