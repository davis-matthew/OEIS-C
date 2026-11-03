/*
 * A050770  Iterated procedure 'composite k added to sum of its prime factors
 *          reaches a prime' yields 3 skipped primes.
 * https://oeis.org/A050770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 45 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iterative process: for each composite k, compute p = k + sum of its prime factors (with multiplicity). If p is prime and there are exactly three primes strictly between k and p, then k is a term of A050770. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

/* Helper: compute sum of prime factors of n (with multiplicity). */
static uint64_t sum_prime_factors(idx_t n)
{
    uint64_t x = n;
    uint64_t sum = 0;
    for (uint64_t p = 2; p * p <= x; ++p)
    {
        while (x % p == 0)
        {
            sum += p;
            x /= p;
        }
    }
    if (x > 1)
        sum += x;
    return sum;
}

seq_t a(idx_t n)
{
    /* Static array holding computed terms. */
    static seq_t *seq = NULL;
    static size_t len = 0;
    static size_t cap = 0;

    /* Bookkeeping of next composite to test. */
    static idx_t next_k = 4;

    while (len <= n)
    {
        idx_t k = next_k;

        /* Scan composites until we find a qualifying term. */
        for (;; ++k)
        {
            if (is_prime_big((seq_t)k))
                continue;   /* skip primes */

            uint64_t s = sum_prime_factors(k);
            idx_t p = k + s;

            if (!is_prime_big((seq_t)p))
                continue;   /* end must be prime */

            /* Count primes strictly between k and p, stop if >3. */
            unsigned cnt = 0;
            for (idx_t x = k + 1; x < p; ++x)
            {
                if (is_prime_big((seq_t)x))
                {
                    if (++cnt > 3)
                        break;
                }
            }

            if (cnt == 3)
            {
                /* Store the term. */
                if (len == cap)
                {
                    size_t new_cap = (cap == 0) ? 16 : cap * 2;
                    seq = mem_resize(seq_t, seq, new_cap);
                    cap = new_cap;
                }
                seq[len++] = (seq_t)k;

                /* Next search starts after this term. */
                next_k = k + 1;
                break;   /* inner loop break, outer loop continues if needed */
            }
        }
    }

    return seq[n];
}
