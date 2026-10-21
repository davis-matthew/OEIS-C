/*
 * A216473  a(n) = smallest m such that 2n-1 | (10^m-1)/9, or 0 if no such m
 *          exists.
 * https://oeis.org/A216473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that 2n-1 | (10^m-1)/9, or 0 if no such m
 *            exists
 * Range:     u8 n <= 14, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

/* compare function for qsort on uint64_t */
static int cmp_uint64(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

/* recursive generation of all divisors of a number */
static void gen_divs(const uint64_t primes[], const int exps[], int idx,
                     size_t pc, uint64_t current,
                     uint64_t divs[], size_t *cnt) {
    if (idx == pc) {
        divs[*cnt] = current;
        (*cnt)++;
        return;
    }
    uint64_t val = current;
    for (int i = 0; i <= exps[idx]; ++i) {
        gen_divs(primes, exps, idx + 1, pc, val, divs, cnt);
        val *= primes[idx];
    }
}

seq_t a(idx_t n) {
    /* guard for index < 1 */
    if (n < 1) return (seq_t)0;

    /* 2n-1 must not be divisible by 5 else gcd(10,9*(2n-1))≠1 */
    if (((2 * n - 1) % 5) == 0) {
        return (seq_t)0;
    }

    /* M = 9 * (2n-1) */
    uint64_t odd  = 2 * n - 1;
    uint64_t Mui  = 9 * odd;
    seq_t M = (seq_t)Mui;

    /* compute Euler's totient phi(M) */
    uint64_t phi = Mui;
    uint64_t tmp = Mui;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            phi = phi / p * (p - 1);
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {
        phi = phi / tmp * (tmp - 1);
    }

    /* factor phi into primes and exponents */
    uint64_t primes[16];
    int exps[16];
    size_t pc = 0;
    tmp = phi;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            primes[pc] = p;
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            exps[pc] = e;
            ++pc;
        }
    }
    if (tmp > 1) {
        primes[pc] = tmp;
        exps[pc]  = 1;
        ++pc;
    }

    /* generate all divisors of phi */
    uint64_t divs[256];
    size_t divs_cnt = 0;
    gen_divs(primes, exps, 0, pc, 1, divs, &divs_cnt);

    /* sort divisors */
    qsort(divs, divs_cnt, sizeof(uint64_t), cmp_uint64);

    /* find smallest d with 10^d ≡ 1 mod M */
    seq_t base10 = (seq_t)10;
    seq_t one = (seq_t)1;
    for (size_t i = 0; i < divs_cnt; ++i) {
        uint64_t d = divs[i];
        if (mod_pow(base10, d, M) == one) {
            return (seq_t)d;
        }
    }

    /* should not happen, but return 0 as safe fallback */
    return (seq_t)0;
}
