/*
 * A052017  Primes with digits in ascending order that differ exactly by 1.
 * https://oeis.org/A052017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: full, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute all primes whose decimal digits are an increasing run of consecutive integers
   (e.g., 2, 3, 5, 7, 23, 67, 89, 4567, 23456789, …).  The function
   returns the n‑th such prime (n = 1 for the first term).  The primes are
   generated once and cached; subsequent calls are O(1).  For seq_t types that
   cannot represent a candidate value, the candidate is skipped. */
#include <stdlib.h>
#include <stdbool.h>
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

static int cmp_seq(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

static seq_t primes[50];
static idx_t count = 0;

static void init_primes(void)
{
    if (count != 0) return;                     /* already done */

    const size_t seq_bytes = sizeof(seq_t);
    const uint64_t max_val_u64 =
        (seq_bytes <= 8) ? (uint64_t)((seq_t)~(seq_t)0) : UINT64_MAX;
    const bool allow_all = (seq_bytes > 8);

    idx_t idx = 0;
    for (idx_t start = 1; start <= 9; ++start) {
        uint64_t val = 0;
        for (idx_t d = start; d <= 9; ++d) {
            val = val * 10 + (uint64_t)d;
            if (!allow_all && val > max_val_u64) continue;
            seq_t v = (seq_t)val;
            if (is_prime_big(v)) primes[idx++] = v;
        }
    }

    if (idx > 0)
        mem_qsort(primes, (size_t)idx, sizeof(seq_t), cmp_seq);
    count = idx;
}

seq_t a(idx_t n)
{
    init_primes();
    if (n == 0 || n > count) return (seq_t)0;
    return primes[n - 1];
}
