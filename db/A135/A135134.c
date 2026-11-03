/*
 * A135134  Emirps that use all of the prime digits 2,3,5,7 exactly once.
 * https://oeis.org/A135134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   Emirps that use all prime digits 2,3,5,7 exactly once (generated
 *            by checking all 24 permutations)
 * Range:     u8 none, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: full, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static int seq_cmp(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

static seq_t seq_arr[4];
static idx_t seq_count = 0;
static bool seq_initialized = false;

static int cmp_seq(const void *a, const void *b)
{
    return seq_cmp(a, b);
}

seq_t a(idx_t n)
{
    if (!seq_initialized) {
        /* generate all permutations of digits 2,3,5,7 */
        int digits[4] = {2, 3, 5, 7};
        idx_t m = 0;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) if (j != i) {
                for (int k = 0; k < 4; ++k) if (k != i && k != j) {
                    for (int l = 0; l < 4; ++l) if (l != i && l != j && l != k) {
                        uint64_t num = 0;
                        int d[4] = {digits[i], digits[j], digits[k], digits[l]};
                        for (int t = 0; t < 4; ++t)
                            num = num * 10 + d[t];
                        uint64_t rev = 0;
                        for (int t = 3; t >= 0; --t)
                            rev = rev * 10 + d[t];
                        if (is_prime_big((seq_t)num) && is_prime_big((seq_t)rev)) {
                            if (m < 4)
                                seq_arr[m++] = (seq_t)num;
                        }
                    }
                }
            }
        }
        mem_qsort(seq_arr, m, sizeof(seq_t), cmp_seq);
        seq_count = m;
        seq_initialized = true;
    }
    if (n <= seq_count)
        return seq_arr[n - 1];
    else
        return (seq_t)0;
}
