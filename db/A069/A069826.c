/*
 * A069826  Numbers k such that sigma(k^2-k-1) = k*(k+1).
 * https://oeis.org/A069826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = t^2 - 1 where t >= 3 is the n‑th integer such that both
 *            t^2 + t - 1 and t^2 - t - 1 are prime. The values k are exactly
 *            the solutions of sigma(k^2 - k - 1) = k*(k+1)
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Validated: standard, pass, 56 terms through n = 374 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* OEIS offset 1 */

    /* Cache of discovered terms */
    static seq_t *cache = NULL;
    static size_t len   = 0;
    static size_t cap   = 0;
    static idx_t  t_max = 2;        /* last t that was examined */

    /* Initialise cache lazily */
    if (cap == 0) {
        cap = 64;
        cache = mem_new(seq_t, cap);
    }

    /* Generate terms until we have at least n of them */
    while (len < n) {
        ++t_max;
        /* Compute t^2, p1, p2 */
        seq_t t_val = (seq_t)t_max;
        seq_t t_sq  = t_val * t_val;
        seq_t p1    = t_sq + t_val - 1;
        seq_t p2    = t_sq - t_val - 1;

        /* Both must be prime */
        if (is_prime_big(p1) && is_prime_big(p2)) {
            /* Store k = t^2 - 1 */
            if (len == cap) {
                cap <<= 1;
                cache = mem_resize(seq_t, cache, cap);
            }
            cache[len++] = t_sq - 1;
        }
    }

    return cache[n - 1];
}
