/*
 * A106491  Total number of bases and exponents in Quetian Superfactorization
 *          of n, including the unity-exponents at the tips of branches.
 * https://oeis.org/A106491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (# of distinct prime factors of n) + Σ a(e_i), for * all
 *            exponents e_i in the prime factorization of n. * a(1) = 1. This
 *            counts every base and every unity exponent in the * Quetian
 *            superfactorization tree
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static struct cache_entry {
    idx_t key;   /* argument n */
    seq_t val;   /* a(n) */
} cache[2048];
static idx_t cache_size = 0;

/* Recursive helper that computes a(n). */
static seq_t a_rec(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* Check cache */
    for (idx_t i = 0; i < cache_size; ++i) {
        if (cache[i].key == n) return cache[i].val;
    }

    /* Factor n and collect exponents */
    idx_t exps[64];
    idx_t exps_cnt = 0;
    idx_t tmp = n;

    /* factor 2 */
    if (tmp % 2 == 0) {
        idx_t e = 0;
        while (tmp % 2 == 0) { tmp /= 2; ++e; }
        exps[exps_cnt++] = e;
    }

    /* odd factors */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            exps[exps_cnt++] = e;
        }
    }

    if (tmp > 1) {          /* remaining prime factor of exponent 1 */
        exps[exps_cnt++] = 1;
    }

    /* compute a(n) = m + Σ a(e_i) */
    seq_t res = (seq_t)exps_cnt;
    for (idx_t i = 0; i < exps_cnt; ++i) {
        res += a_rec(exps[i]);
    }

    /* store in cache */
    if (cache_size < 2048) {
        cache[cache_size].key = n;
        cache[cache_size].val = res;
        ++cache_size;
    }

    return res;
}

/* Public function, with external linkage as required. */
seq_t a(idx_t n)
{
    return a_rec(n);
}
