/*
 * A180022  Primes that can be obtained from other primes by interchanging the
 *          first and last digits. The source prime and the resulting prime
 *          are written consecutively.
 * https://oeis.org/A180022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A180022: primes that can be obtained from other primes by swapping the
 * first and last decimal digits.  Each such prime pair contributes two
 * consecutive terms: the source prime followed by the swapped prime.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/num_digits.h"
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Cache of generated terms */
static seq_t *cache = NULL;
static size_t  cache_len = 0, cache_cap = 0;
static seq_t  next_candidate = 2;   /* next integer to test for primality */

/* Ensure that at least n terms have been generated */
static void ensure_terms(idx_t n)
{
    if (cache_len >= n)
        return;

    if (cache_cap == 0) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
        if (!cache)
            return;   /* allocation failed: terminate silently */
    }

    while (cache_len < n) {
        /* Skip non‑primes */
        if (!is_prime_big(next_candidate))
            { ++next_candidate; continue; }

        /* Ignore one‑digit primes (they would give identical swap) */
        int d = num_digits(next_candidate);
        if (d < 2) {
            ++next_candidate;
            continue;
        }

        /* Compute the swapped number */
        seq_t pow10_d_minus1 = pow10(d - 1);
        seq_t first  = next_candidate / pow10_d_minus1;
        seq_t last   = next_candidate % 10;
        seq_t middle = (next_candidate % pow10_d_minus1) / 10;
        seq_t swapped = last * pow10_d_minus1 + middle * 10 + first;

        /* If both primes, append them to the cache */
        if (is_prime_big(swapped)) {
            if (cache_len + 2 > cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                if (!tmp)
                    return;   /* allocation failed */
                cache = tmp;
            }
            cache[cache_len++] = next_candidate;
            cache[cache_len++] = swapped;
        }

        ++next_candidate;
    }
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset zero */

    ensure_terms(n);
    return cache[n - 1];
}
