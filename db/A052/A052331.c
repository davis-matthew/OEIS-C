/*
 * A052331  Inverse of A052330; A binary encoding of Fermi-Dirac factorization
 *          of n, shown in decimal.
 * https://oeis.org/A052331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 40, u32 n <= 102, u64 n <= 262,
 *            u128 n <= 640, big any n
 * Helpers:   compare_uint64, power_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A052331: inverse of A052330 – binary encoding of the Fermi–Dirac
   factorisation of n, in decimal.
   Implementation: factorise n, for every prime factor p^e write e in binary
   as a sum of distinct powers of two, and sum 2^(index(p^(2^k))-1) over
   all such powers.  The indices are taken from the ordered sequence
   of all numbers of the form p^(2^k).  The procedure pre‑computes all
   required indices up to the current n. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/compare_uint64.h"
#include "../utils/power_u64.h"

/* static state for on‑the‑fly precomputation */
static idx_t _max_precomputed_limit = 0;
static uint8_t * _is_prime = NULL;          /* is_prime[i] = 0/1 up to limit */
static size_t * _index_map = NULL;          /* index_map[value] = rank (1‑based) */

/* pre‑compute primes, prime‑powers and their indices up to 'limit' */
static void precompute(idx_t limit)
{
    if (limit <= _max_precomputed_limit)
        return;  /* already done */

    free(_is_prime);
    free(_index_map);

    _is_prime = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    _index_map = (size_t*)calloc((limit + 1), sizeof(size_t)); /* zero init */

    for (idx_t i = 0; i <= limit; ++i)
        _is_prime[i] = 1;
    if (limit >= 0) _is_prime[0] = 0;
    if (limit >= 1) _is_prime[1] = 0;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (_is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                _is_prime[j] = 0;

    /* collect all prime powers p^(2^k) <= limit */
    uint64_t *vals = (uint64_t*)malloc((limit + 1) * sizeof(uint64_t));
    size_t vals_cnt = 0;

    for (idx_t p = 2; p <= limit; ++p)
        if (_is_prime[p]) {
            uint64_t exp = 1;          /* exponent: 1,2,4,8,...   */
            while (1) {
                uint64_t val = 1;
                for (uint64_t k = 0; k < exp; ++k)
                    val *= (uint64_t)p;
                if (val > (uint64_t)limit)
                    break;
                vals[vals_cnt++] = val;

                if (exp > limit / ((uint64_t)p)) /* avoid overflow of exp*p */
                    break;
                exp <<= 1;
            }
        }

    qsort(vals, vals_cnt, sizeof(uint64_t), compare_uint64);
    for (size_t i = 0; i < vals_cnt; ++i)
        _index_map[vals[i]] = i + 1;  /* ranks start at 1 */

    free(vals);
    _max_precomputed_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    precompute(n);  /* ensure prime‑powers up to n are mapped */

    seq_t result = 0;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint32_t exp = 0;
            while (temp % p == 0) {
                ++exp;
                temp /= p;
            }

            uint32_t bitpos = 0;
            uint32_t rem = exp;
            while (rem) {
                if (rem & 1U) { /* exponent equals 2^bitpos */
                    uint64_t exponent = ((uint64_t)1) << bitpos;
                    uint64_t val = power_u64((uint64_t)p, exponent);
                    size_t idx_val = _index_map[val];
                    result += ((seq_t)1) << (idx_val - 1);
                }
                rem >>= 1U;
                ++bitpos;
            }
        }
    }

    if (temp > 1) { /* remaining prime factor, exponent = 1 */
        size_t idx_val = _index_map[temp];
        result += ((seq_t)1) << (idx_val - 1);
    }

    return result;
}
