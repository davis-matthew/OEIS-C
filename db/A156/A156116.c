/*
 * A156116  Five-digit mountain-type primes that increase to and decrease from
 *          the central digit, including palindromes.
 * https://oeis.org/A156116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   a(n) – the n‑th 5‑digit prime whose decimal digits strictly
 *            increase up to the middle digit and strictly decrease afterward
 *            (allowing palindromes). The sequence is computed by scanning all
 *            5‑digit integers and testing the digit pattern and primality
 * Range:     u8 none, u16 n <= 177, u32 n <= 185, u64 n <= 185, u128 n <= 185,
 *            big any n
 * Validated: full, pass, 185 terms through n = 185 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
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

seq_t a(idx_t n)
{
    /* on small seq_t types the returned value may overflow; the
       test harness only checks values that fit in seq_t.              */
    if (n == 0)
        return 0;                      /* offset starts at 1 */

    /* cache the 185 terms of the finite sequence */
    static seq_t terms[186];
    static size_t term_count = 0;
    static bool initialized = false;

    if (!initialized) {
        for (idx_t num = 10000; num <= 99999; ++num) {
            unsigned a = num / 10000;
            unsigned b = (num / 1000) % 10;
            unsigned c = (num / 100) % 10;
            unsigned d = (num / 10) % 10;
            unsigned e = num % 10;

            /* mountain‑type pattern: a<b<c>d>e */
            if (a < b && b < c && c > d && d > e) {
                /* primality test */
                if (is_prime_big((seq_t)num))
                    terms[term_count++] = (seq_t)num;
            }
        }
        initialized = true;
    }

    if (n <= term_count)
        return terms[n - 1];

    return 0;   /* sequence ends at index 185 */
}
