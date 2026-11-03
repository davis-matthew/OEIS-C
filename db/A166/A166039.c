/*
 * A166039  Sums of three consecutive nonprimes A141468.
 * https://oeis.org/A166039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n) + b(n+1) + b(n+2) where b(k) is the k‑th nonprime
 *            (0,1,4,6,8,9,10,…)
 * Range:     u8 n <= 62, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 73 terms through n = 4379 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

static seq_t *nonprimes = NULL;   /* array of generated nonprimes */
static size_t nonprime_cap = 0;   /* current capacity of the array */
static size_t nonprime_len = 0;   /* number of elements currently stored */
static uint64_t next_num = 0;     /* next integer to test for primality */

/* Grow the nonprime array until it holds at least 'target' elements. */
static void ensure_nonprimes(idx_t target)
{
    if ((idx_t)nonprime_len >= target) return;

    while ((idx_t)nonprime_len < target) {
        seq_t val = (seq_t)next_num;
        if (!is_prime_big(val)) {           /* val is a nonprime: 0,1,4,6,… */
            if (nonprime_len == nonprime_cap) {
                size_t new_cap = nonprime_cap ? nonprime_cap * 2 : 1024;
                seq_t *tmp = mem_resize(seq_t, nonprimes, new_cap);
                if (!tmp) exit(EXIT_FAILURE);
                nonprimes = tmp;
                nonprime_cap = new_cap;
            }
            nonprimes[nonprime_len++] = val;
        }
        next_num++;
    }
}

/* Return a(n) for n >= 1.  For n < 1 the function returns 0 (undefined). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* We need the (n), (n+1), and (n+2)‑th nonprimes. */
    ensure_nonprimes(n + 2);

    size_t i = (size_t)(n - 1);          /* 0‑based index for b(n) */
    seq_t sum = nonprimes[i] + nonprimes[i + 1] + nonprimes[i + 2];
    return sum;
}
