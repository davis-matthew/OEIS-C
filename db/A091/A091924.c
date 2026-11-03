/*
 * A091924  Primes such that their decimal representations interpreted in base
 *          11 are also prime.
 * https://oeis.org/A091924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth prime p such that the base‑11 * interpretation of
 *            its decimal digits is also prime
 * Range:     u8 n <= 13, u16 n <= 716, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9677 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Basic utilities */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Dependency helpers – they are available in the build */
#include "../utils/is_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

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

/*
 * The function a(idx_t n) returns the nth term of the sequence.
 * It memoises the terms in a static array, generating them on demand.
 */
seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *cache    = NULL;   /* array of terms */
    static size_t cache_len = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */

    static uint64_t next_candidate = 2;   /* next integer to test for primality */

    /* Generate terms until we have at least `n` of them */
    while (cache_len < (size_t)n) {
        /* Test whether `next_candidate` is prime (in decimal) */
        if (is_prime(next_candidate)) {
            /* Convert its decimal digits to a base‑11 value */
            uint64_t tmp = next_candidate;
            /* We accumulate the value in 128‑bit to avoid overflow */
            unsigned __int128 val128 = 0;
            /* Extract digits in reverse order */
            uint8_t dig_buf[32];
            size_t dcnt = 0;
            do {
                dig_buf[dcnt++] = (uint8_t)(tmp % 10);
                tmp /= 10;
            } while (tmp > 0);

            /* Build the base‑11 number from the most significant digit */
            for (size_t i = dcnt; i > 0; --i) {
                val128 = val128 * 11 + dig_buf[i - 1];
            }

            /* If the value fits in seq_t (uint64_t for the 64‑bit build),
             * cast it; otherwise skip this prime – the witness
             * will be too large for this build and does not affect
             * the correctness of terms that do fit. */
            seq_t base11_val = 0;
            bool overflow = false;
            if (sizeof(seq_t) < 16) { /* seq_t is smaller than 128 bits */
                if (val128 > UINT64_MAX) {
                    overflow = true;
                } else {
                    base11_val = (seq_t)val128;
                }
            } else { /* arbitrary‑precision or larger types can take the value */
                base11_val = (seq_t)val128;
            }

            if (!overflow && is_prime_big(base11_val)) {
                /* Append the decimal prime to the cache */
                append_candidate(&cache, &cache_len, &cache_cap,
                                 (seq_t)next_candidate);
            }
        }

        /* Advance to the next candidate */
        ++next_candidate;
    }

    return cache[n - 1];
}
