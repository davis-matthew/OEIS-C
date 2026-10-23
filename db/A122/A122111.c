/*
 * A122111  Self-inverse permutation of the positive integers induced by
 *          partition enumeration in A112798 and partition conjugation.
 * https://oeis.org/A122111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) is obtained by - replacing each prime factor p_i of n by
 *            its index i - taking the conjugate partition of that multiset of
 *            indices - replacing each part j with the j‑th prime and
 *            multiplying them. The algorithm below implements this directly
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 130, u64 n <= 310,
 *            u128 n <= 718, big any n
 * Helpers:   factor_u64, get_prime, prime_pi, seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"
#include "../utils/get_prime.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_one.h"

static int cmp_idx(const void *a, const void *b)
{
    idx_t va = *(const idx_t *)a;
    idx_t vb = *(const idx_t *)b;
    return (va > vb) - (va < vb);
}

seq_t a(idx_t n)
{
    if (n == 1) return seq_one();

    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t pcnt;
    factor_u64(n, primes, exps, &pcnt);

    idx_t indices[128];
    size_t idx_cnt = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        idx_t idx = (idx_t)prime_pi(primes[i]);
        for (uint64_t e = 0; e < exps[i]; ++e) {
            /* 128 is safely larger than the max number of prime factors
               of a 64‑bit integer (≤ 64). */
            indices[idx_cnt++] = idx;
        }
    }

    if (idx_cnt == 0) return seq_one();

    qsort(indices, idx_cnt, sizeof(idx_t), cmp_idx);

    seq_t result = seq_one();
    idx_t remaining = idx_cnt;
    idx_t last_part = 1;
    size_t pos = 0;
    while (pos < idx_cnt) {
        idx_t current = indices[pos];
        size_t freq = 0;
        while (pos < idx_cnt && indices[pos] == current) {
            ++freq;
            ++pos;
        }
        idx_t length = current - last_part + 1;
        seq_t prime_val = (seq_t)get_prime(remaining);
        seq_t term = seq_pow(prime_val, length);
        result = seq_mul(result, term);
        remaining -= freq;
        last_part = current + 1;
    }
    return result;
}
