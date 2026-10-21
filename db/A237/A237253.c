/*
 * A237253  Number of ordered ways to write n = k + m with k > 0 and m > 0
 *          such that phi(k) - 1, phi(k) + 1 and prime(prime(prime(m))) - 2
 *          are all prime, where phi(.) is Euler's totient function.
 * https://oeis.org/A237253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (k,m) with k+m=n, k>0, m>0, such
 *            that phi(k)-1, phi(k)+1, and prime(prime(prime(m)))-2 are all
 *            prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- External utilities ---------- */
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */
#include "../utils/build_primes.h" /* void build_primes(idx_t limit, idx_t **out, size_t *cnt); */

/* ---------- Global prime table ---------- */
static idx_t *prime_list = NULL;   /* list of primes in increasing order */
static size_t prime_cnt  = 0;      /* number of primes stored */

/* ---------- Prime initialization ---------- */
static void init_primes(void)
{
    if (prime_list != NULL)
        return;                    /* already initialized */

    /* An upper bound that guarantees the list contains enough primes for
       all nested prime lookups up to n≈10000. */
    const idx_t PRIME_LIMIT = 26000000ULL;
    build_primes(PRIME_LIMIT, &prime_list, &prime_cnt);
}

/* ---------- Euler totient (using the global prime list) ---------- */
static uint64_t phi_uint64(uint64_t n)
{
    uint64_t result = n;
    uint64_t temp = n;

    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            while (temp % p == 0)
                temp /= p;
            result -= result / p;
        }
    }

    if (temp > 1)        /* remaining prime factor */
        result -= result / temp;

    return result;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* below offset */

    init_primes();

    uint64_t count = 0;

    for (idx_t k = 1; k < n; ++k) {
        idx_t m = n - k;

        /* ---- Totient condition on k ---- */
        uint64_t phi_k = phi_uint64((uint64_t)k);
        if (phi_k <= 2)                     /* phi_k – 1 <= 1 cannot be prime */
            continue;

        if (!is_prime((idx_t)(phi_k - 1)) || !is_prime((idx_t)(phi_k + 1)))
            continue;

        /* ---- Nested prime condition on m ---- */
        if (m == 0)                         /* impossible, but keep safety */
            continue;

        /* m-th prime (1‑based index) */
        if (m > (idx_t)prime_cnt)
            continue;   /* not enough primes precomputed */
        idx_t p1 = prime_list[m - 1];

        /* (p1)-th prime */
        if (p1 < 1 || (idx_t)p1 > (idx_t)prime_cnt)
            continue;
        idx_t p2 = prime_list[p1 - 1];

        /* (p2)-th prime */
        if (p2 < 1 || (idx_t)p2 > (idx_t)prime_cnt)
            continue;
        idx_t p3 = prime_list[p2 - 1];

        if (p3 <= 2)                 /* p3-2 <= 0 cannot be prime */
            continue;

        if (!is_prime(p3 - 2))
            continue;

        /* All conditions satisfied */
        ++count;
    }

    return (seq_t)count;
}
