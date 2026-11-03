/*
 * A153660  Pairs of terms of A038512 which differ by 2.
 * https://oeis.org/A153660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2048, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9896 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A153660 – pairs of consecutive terms of A038512 that differ by 2
   Algorithm: iterate over odd numbers, test membership in A038512
   (composite and all prime factors ≥ 13) using is_prime_big.
   When two consecutive such numbers differ by 2, store them in a static
   cache and return the n‑th term (1‑based). */

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

static inline bool in_A038512(seq_t x)
{
    /* A038512 numbers are odd composites with prime factors ≥ 13. */
    if (x <= 1u)
        return false;
    /* Reject if divisible by any prime < 13. */
    if (x % 3u == 0u) return false;
    if (x % 5u == 0u) return false;
    if (x % 7u == 0u) return false;
    if (x % 11u == 0u) return false;
    /* Reject primes ≥ 13. */
    if (is_prime_big(x))
        return false;
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static seq_t next_candidate = 13; /* first odd number ≥ 13 */

    while (terms_len < n)
    {
        seq_t cand = next_candidate;
        next_candidate += 2;  /* next odd number */

        if (!in_A038512(cand))
            continue;

        seq_t cand_plus2 = cand + 2u;
        if (!in_A038512(cand_plus2))
            continue;

        /* Ensure capacity for two more terms. */
        if (terms_len + 2 > terms_cap)
        {
            size_t new_cap = terms_cap ? terms_cap * 2 : 64;
            seq_t *tmp = mem_resize(seq_t, terms, new_cap);
            if (!tmp)
                exit(1); /* allocation failure */
            terms = tmp;
            terms_cap = new_cap;
        }

        terms[terms_len++] = cand;
        terms[terms_len++] = cand_plus2;
    }

    return terms[n - 1];
}
