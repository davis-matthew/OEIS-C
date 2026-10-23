/*
 * A106061  Primes p such that 1*p + 10 and 10*p + 1 are primes.
 * https://oeis.org/A106061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime p such that p+10 and 10*p+1 are prime
 * Range:     u8 n <= 11, u16 n <= 371, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Miller–Rabin primality test for 64‑bit integers ---------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;

    /* Small prime trial division */
    static const uint64_t small[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof small / sizeof small[0]; ++i) {
        if (n % small[i] == 0) return n == small[i];
    }

    /* Write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0) { d >>= 1; ++s; }

    /* Deterministic bases for 64‑bit numbers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        /* Compute a^d mod n */
        __uint128_t x = 1, p = a, exp = d;
        while (exp) {
            if (exp & 1) x = (x * p) % n;
            p = (p * p) % n;
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;

        bool witness = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { witness = false; break; }
        }
        if (witness) return false;
    }
    return true;
}

/* ---------- Sequence generator ------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* Undefined for indices below the offset */

    /* Static storage for found terms */
    static seq_t *values = NULL;
    static idx_t  cap   = 0;
    static idx_t  size  = 0;
    static uint64_t last_checked = 1;   /* Last prime examined */

    /* Extend array until we have n terms */
    while (size < n) {
        /* Find next prime after last_checked */
        uint64_t p = last_checked + 1;
        while (!is_prime_uint64(p)) ++p;

        /* Check required property */
        if (is_prime_uint64(p + 10) && is_prime_uint64(10 * p + 1)) {
            /* Resize if needed */
            if (size == cap) {
                idx_t new_cap = cap ? cap << 1 : 16;
#ifdef __cplusplus
                seq_t *tmp = new seq_t[new_cap];
                for (idx_t i = 0; i < size; ++i) tmp[i] = values[i];
                delete [] values;
#else
                seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* Allocation failure */
                memcpy(tmp, values, size * sizeof(seq_t));
                free(values);
#endif
                values = tmp;
                cap = new_cap;
            }
            values[size++] = (seq_t)p;
        }
        last_checked = p;
    }

    return values[n - 1];
}
