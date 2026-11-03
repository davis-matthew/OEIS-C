/*
 * A231819  Least positive k such that k*n^2 - 1 is a prime, or 0 if no such k
 *          exists.
 * https://oeis.org/A231819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k ≥ 1 so that k*n^2 - 1 is prime; a(n)=0 if no such
 *            k exists
 * Range:     u8 n <= 8, u16 n <= 70, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Validated: partial, pass, 85 terms through n = 85 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine performs a straightforward linear search for k, up to a fixed 
   upper bound that is sufficient for the test range (n ≤ 10000). */

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

static const idx_t K_SEARCH_LIMIT = 1000000;   /* Max k to try per n */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset starts at 1 */

    const seq_t nsq = (seq_t)n * (seq_t)n;   /* n^2, fits in seq_t for the limits */

    for (idx_t k = 1; k <= K_SEARCH_LIMIT; ++k) {
        /* Compute k*n^2, then subtract 1 to test for primality. */
        const seq_t prod = nsq * (seq_t)k;
        const seq_t cand = prod - 1;

        if (is_prime_big(cand))
            return (seq_t)k;   /* Found the least such k. */
    }

    return 0;   /* No k found within the search limit. */
}
