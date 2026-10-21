/*
 * A050332  Number of factorizations of n into distinct numbers with an odd
 *          number of prime factors.
 * https://oeis.org/A050332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of factorizations of n into distinct integers > 1,
 *            each having an odd number of prime factors (counted with
 *            multiplicity). The empty factorization is counted for n = 1
 * Range:     u8 n <= 2519, u16 n <= 2519, u32 n <= 2519, u64 n <= 2519,
 *            u128 n <= 2519, big any n
 * Helpers:   odd_prime_factor_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A050332) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/odd_prime_factor_count.h"

typedef uint64_t idx_t;   /* index type (the build supplies the real type) */

/* ---------- Helpers for factoring and divisor enumeration ---------- */

static void factor_uint64(uint64_t x, uint64_t *pr, int *exp, size_t *cnt) {
    size_t c = 0;
    if (x % 2 == 0) {
        pr[c] = 2;
        exp[c] = 0;
        while (x % 2 == 0) { exp[c]++; x /= 2; }
        c++;
    }
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            pr[c] = p;
            exp[c] = 0;
            while (x % p == 0) { exp[c]++; x /= p; }
            c++;
        }
    }
    if (x > 1) {
        pr[c] = x;
        exp[c] = 1;
        c++;
    }
    *cnt = c;
}

/* A small static container for divisors (enough for n <= 10^6). */
#define MAX_DIV 512
typedef struct {
    uint64_t arr[MAX_DIV];
    int      cnt;
} DivBuffer;

/* Recursively generate all divisors of n using its prime factorization. */
static void dfs_divs(int idx, uint64_t cur,
                     const uint64_t *pr, const int *exp, size_t num,
                     DivBuffer *buf) {
    if (idx == num) {
        buf->arr[buf->cnt++] = cur;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t val = cur;
    for (int e = 0; e <= exp[idx]; ++e) {
        dfs_divs(idx + 1, val, pr, exp, num, buf);
        val *= p;
    }
}

static void get_divisors(uint64_t n, DivBuffer *buf) {
    uint64_t pr[32];
    int      exp[32];
    size_t   num;
    factor_uint64(n, pr, exp, &num);
    buf->cnt = 0;
    dfs_divs(0, 1, pr, exp, num, buf);
    /* The buffer now contains all divisors, including 1. */
    /* Sort divisors ascending (simple insertion sort for small size). */
    for (int i = 1; i < buf->cnt; ++i) {
        uint64_t key = buf->arr[i];
        int j = i - 1;
        while (j >= 0 && buf->arr[j] > key) {
            buf->arr[j + 1] = buf->arr[j];
            --j;
        }
        buf->arr[j + 1] = key;
    }
}

/* ---------- Main counting routine ---------- */

static uint64_t count_partitions(uint64_t rem, uint64_t last) {
    uint64_t total = 0;
    DivBuffer buf;
    get_divisors(rem, &buf);
    for (int i = 0; i < buf.cnt; ++i) {
        uint64_t d = buf.arr[i];
        if (d <= last) continue;          /* enforce strictly increasing to keep distinct */
        if (!odd_prime_factor_count(d)) continue; /* each factor must have odd PF count */

        if (d == rem) {
            /* last factor completed the factorization */
            ++total;
        } else {
            total += count_partitions(rem / d, d);
        }
    }
    return total;
}

/* ---------- Public function ---------- */

seq_t a(idx_t n) {
    if (n <= 1) return 1;                /* special case: a(1) = 1 */
    return (seq_t)count_partitions((uint64_t)n, 1);
}
