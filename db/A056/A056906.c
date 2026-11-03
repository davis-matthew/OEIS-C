/*
 * A056906  Numbers k such that 36*k^2 + 5 is prime.
 * https://oeis.org/A056906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 961 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 36*k^2 + 5 is prime
   The function builds a static table incrementally,
   adding each k that satisfies the prime condition.
   No external dependencies beyond the provided is_prime_big
   routine.  This works for all supported seq_t sizes.  */

#include <stdbool.h>
#include <stddef.h>
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

/* Static storage for found terms – more than enough for the
   required n ≤ 10 000 (≈350 000 k values are enough).  */
#define MAX_TERMS 200000
static seq_t terms[MAX_TERMS];
static size_t terms_len = 0;
static idx_t next_k = 0;

seq_t a(idx_t n)
{
    /* Off‑by‑one: OEIS offset is 1; ignore n==0.  */
    if (n == 0) return 0;

    /* Build the list until we have at least n elements.  */
    while (terms_len < n)
    {
        seq_t kk = (seq_t)next_k;
        seq_t m  = ((seq_t)36) * kk * kk + (seq_t)5;
        if (is_prime_big(m))
            terms[terms_len++] = kk;
        ++next_k;
    }
    return terms[n - 1];
}
