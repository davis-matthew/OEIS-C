/*
 * A054505  Log_b 2 where b is smallest primitive root (A001918) mod n-th
 *          prime.
 * https://oeis.org/A054505
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = log_b 2 (mod p), where p = n-th prime (n ≥ 2) and b is
 *            the smallest primitive root modulo p. Returns 0 for n < 2
 *            (outside the OEIS offset)
 * Range:     u8 n <= 54, u16 n <= 6603, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, _smallest_prime_factor, get_prime, mod_mul,
 *            mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/_smallest_prime_factor.h"
#include "../utils/mod_pow.h"
#include "../utils/mod_mul.h"

/* Factor p-1 into its distinct prime factors.
   The array 'factors' must be large enough (≤ 32). */
static void factor_distinct(uint64_t n, uint64_t *factors, size_t *cnt)
{
    *cnt = 0;
    while (n > 1) {
        uint64_t f = _smallest_prime_factor(n);
        factors[(*cnt)++] = f;
        while (n % f == 0) n /= f;
    }
}

/* Smallest primitive root modulo prime p (p > 2). */
static seq_t smallest_primitive_root(uint64_t p)
{
    uint64_t factors[32];
    size_t fc;
    factor_distinct(p - 1, factors, &fc);

    for (uint64_t g = 2; g < p; ++g) {
        bool ok = true;
        for (size_t i = 0; i < fc; ++i) {
            uint64_t q = factors[i];
            seq_t exp = (seq_t)((p - 1) / q);
            seq_t res = mod_pow((seq_t)g, (uint64_t)exp, (seq_t)p);
            if (res == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return (seq_t)g;
    }
    return 0; /* Should never happen for prime p > 2 */
}

/* Discrete logarithm using baby‑step giant‑step.
   Finds k such that base^k ≡ target (mod mod), 0 ≤ k < mod-1. */
static seq_t discrete_log(seq_t base, seq_t target, seq_t mod)
{
    uint64_t p = (uint64_t)mod;           /* p fits in 64 bits */
    uint64_t m = _floor_isqrt_u64(p - 1) + 1;

    /* Use a simple open‑addressing hash table. */
    size_t tblsize = 1;
    while (tblsize < 2 * m) tblsize <<= 1;
    size_t mask = tblsize - 1;

    struct entry { seq_t key; idx_t val; } *table =
        (struct entry *)calloc(tblsize, sizeof(*table));
    if (!table) return 0; /* allocation failure, unlikely */

    /* Baby steps: base^j */
    seq_t baby = 1;
    for (size_t j = 0; j < m; ++j) {
        uint64_t h = ((uint64_t)baby * 1315423911ull) & mask;
        while (table[h].key != 0) h = (h + 1) & mask;
        table[h].key = baby;
        table[h].val = (idx_t)j;
        baby = (seq_t)mod_mul((uint64_t)baby, (uint64_t)base, p);
    }

    /* Compute base^(-m) mod mod */
    seq_t inv_base_m = mod_pow(base, (unsigned long)(p - 1 - m), mod);

    seq_t cur = target % mod;
    for (size_t i = 0; i <= m; ++i) {
        uint64_t h = ((uint64_t)cur * 1315423911ull) & mask;
        while (table[h].key != 0) {
            if (table[h].key == cur) {
                seq_t result = (seq_t)(i * m + table[h].val);
                free(table);
                return result;
            }
            h = (h + 1) & mask;
        }
        cur = (seq_t)mod_mul((uint64_t)cur, (uint64_t)inv_base_m, p);
    }
    free(table);
    return 0; /* should never happen for a prime modulus */
}

/* Public sequence function required by the build. */
seq_t a(idx_t n)
{
    if (n < 2) return 0; /* outside of OEIS offset */

    uint32_t p32 = get_prime(n);   /* n-th prime, fits in 32 bits */
    uint64_t p  = (uint64_t)p32;
    seq_t    prime = (seq_t)p;

    seq_t g = smallest_primitive_root(p);
    seq_t log2 = discrete_log(g, (seq_t)2u, prime);
    return log2;
}
