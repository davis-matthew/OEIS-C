/*
 * A141587  Nonprimes of the form c(c(c(i)-3)-3)-3, where A002808(i)=c(i)=i-th
 *          composite.
 * https://oeis.org/A141587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nonprimes of the form c(c(c(i)-3)-3)-3 where c(k) is the k‑th composite number.
   The function a(n) returns the n‑th nonprime value in increasing order of i (1‑based). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

#define MAX_COMPOSITES 500000   /* generous upper bound for compiled sequences */

/* Storage for composite numbers (1‑based): composites[0] = c(1) = 4 */
static seq_t composites[MAX_COMPOSITES];
static idx_t composite_count = 0;      /* number of composites stored so far */
static idx_t next_candidate   = 4;     /* next integer to test for compositeness */

/* Ensure composites[0 .. k-2] exist (k >= 1).  Fails with abort() if k > MAX_COMPOSITES. */
static void fill_composites(idx_t k)
{
    while (composite_count < k && composite_count < MAX_COMPOSITES) {
        if (!is_prime_big((seq_t)next_candidate)) {
            composites[composite_count++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    if (composite_count < k) {
        /* We ran out of pre‑allocated space – this should never happen for the intended limits. */
        abort();
    }
}

/* Return the k‑th composite number c(k), with k>=1. */
static seq_t get_c(idx_t k)
{
    if (k == 0) return 0;            /* safety – not used by the algorithm */
    if (k > composite_count) fill_composites(k);
    return composites[k - 1];
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1 – 0 is undefined */
    idx_t found = 0;
    for (idx_t i = 1; ; ++i) {
        seq_t c1 = get_c(i);                  /* c(i) */
        idx_t t1 = (idx_t)(c1 - 3);            /* i1 = c(i) - 3 */
        if (t1 == 0) continue;                 /* safety – not expected */

        seq_t c2 = get_c(t1);                  /* c(i1) */
        idx_t t2 = (idx_t)(c2 - 3);            /* i2 = c(i1) - 3 */
        if (t2 == 0) continue;                 /* safety – not expected */

        seq_t c3 = get_c(t2);                  /* c(i2) */
        seq_t val = c3 - 3;                    /* final value */

        /* 1 is treated as a nonprime */
        if (val == 1 || !is_prime_big(val)) {
            ++found;
            if (found == n) return val;
        }
    }
}
