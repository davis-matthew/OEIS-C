/*
 * A179030  Primes whose only prime digits are 7's.
 * https://oeis.org/A179030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 504, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 907 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculation: search successive primes whose decimal digits contain only
   the prime digit 7 (and no 2,3,5) and at least one 7.  */
#include <stdbool.h>
#include <stdlib.h>

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

/* return true iff n has no prime digits 2,3,5 and contains at least one 7 */
static bool has_allowed_digits(seq_t n)
{
    bool has7 = false;
    seq_t tmp = n;
    while (tmp > 0) {
        int d = (int)(tmp % 10);
        if (d == 2 || d == 3 || d == 5)
            return false;
        if (d == 7)
            has7 = true;
        tmp /= 10;
    }
    return has7;
}

#define CACHE_MAX 20000

static seq_t cache[CACHE_MAX];
static size_t cache_len = 0;
static seq_t next_start = 2;

seq_t a(idx_t n)
{
    /* Fast return if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Generate terms until we have n of them */
    while (cache_len < (size_t)n) {
        seq_t curr = next_start;
        /* Search for the next valid prime */
        while (1) {
            if (has_allowed_digits(curr) && is_prime_big(curr)) {
                if (cache_len == CACHE_MAX) {
                    /* should not happen for n <= 10000 */
                    abort();
                }
                cache[cache_len++] = curr;
                next_start = curr + 1;
                break;
            }
            ++curr;
        }
    }
    return cache[n - 1];
}
