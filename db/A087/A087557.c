/*
 * A087557  a(n) = smallest prime that is the concatenation of two numbers
 *          whose product is n.
 * https://oeis.org/A087557
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest prime that is the concatenation of two numbers
 *            whose product is n‑1. For n < 2 the function returns 0. If no
 *            such prime exists, the value 0 is returned. This matches the
 *            OEIS table with offset 2 (a(2) = 11, a(3) = 0, ...). The
 *            implementation is purely arithmetic and does not rely...
 * Range:     u8 n <= 26, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 72 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

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

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 2.  Return 0 otherwise. */
    if (n < 2)
        return 0;

    /* Target product is n‑1. */
    idx_t m = n - 1;

    seq_t best = 0;                       /* 0 means “none seen yet”. */

    /* Enumerate all divisor pairs d * e = m. */
    /* The loop limits to sqrt(m) using division to avoid overflow. */
    for (idx_t d = 1; d <= m / d; ++d) {
        if (m % d != 0)
            continue;
        idx_t e = m / d;

        /* Candidate 1: concat(d, e). */
        seq_t cand1 = concat_parts(d, e);
        if (is_prime_big(cand1)) {
            if (best == 0 || cmp_seq_t(&cand1, &best) < 0)
                best = cand1;
        }

        /* Candidate 2: concat(e, d) if d != e. */
        if (d != e) {
            seq_t cand2 = concat_parts(e, d);
            if (is_prime_big(cand2)) {
                if (best == 0 || cmp_seq_t(&cand2, &best) < 0)
                    best = cand2;
            }
        }
    }

    return best;   /* 0 if no prime concatenation was found. */
}
