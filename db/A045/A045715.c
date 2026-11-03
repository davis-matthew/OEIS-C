/*
 * A045715  Primes with first digit 9.
 * https://oeis.org/A045715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 127, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 956 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
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

/* Computes the n‑th prime whose first decimal digit is 9 (OEIS A045715).
   a(1) = 97, a(2) = 907, …  */
static int first_digit(idx_t x)
{
    while (x >= 10) x /= 10;
    return (int)x;
}

/* Lazy storage for primes that start with the digit 9. */
static seq_t *candidates = NULL;   /* array of primes */
static size_t cand_len   = 0;      /* number of stored primes */
static size_t cand_cap   = 0;      /* allocated capacity */
static idx_t  search     = 2;      /* next integer to test */

/* Ensure that at least n candidates are stored. */
static void ensure_candidates(idx_t n)
{
    while (cand_len < n)
    {
        /* Move to the next integer whose most significant digit is 9. */
        while (first_digit(search) != 9) ++search;

        /* Test primality using the provided routine. */
        if (is_prime_big((seq_t)search))
        {
            /* Grow the array if required. */
            if (cand_len >= cand_cap)
            {
                size_t new_cap = (cand_cap == 0) ? 16 : cand_cap * 2;
                seq_t *tmp = mem_resize(seq_t, candidates, new_cap);
                if (!tmp) abort();  /* fatal allocation error */
                candidates = tmp;
                cand_cap = new_cap;
            }
            candidates[cand_len++] = (seq_t)search;
        }
        ++search;
    }
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1 – undefined for n = 0 */
    ensure_candidates(n);
    return candidates[n - 1];         /* a(1) is the first stored prime */
}
