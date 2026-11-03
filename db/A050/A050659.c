/*
 * A050659  Numbers n such that number of primes produced according to rules
 *          stipulated in Honaker's A048853 is 8.
 * https://oeis.org/A050659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the nth term of A050659
 * (numbers producing exactly 8 distinct primes when each decimal
 * digit is replaced by all other digits, excluding leading zeros). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
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

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache   = NULL;      /* stored terms */
    static size_t cache_len = 0;       /* number of stored terms */
    static size_t cache_cap = 0;       /* allocated capacity */
    static idx_t   next      = 1;      /* next integer to test */

    /* Expand cache until we have at least n terms */
    while (cache_len < n)
    {
        /* current candidate number */
        seq_t cand = (seq_t)next;

        /* Extract decimal digits (little‑endian) */
        seq_t tmp = cand;
        unsigned char digits[32];
        size_t dlen = 0;
        if (tmp == 0)
        {
            digits[dlen++] = 0;
        }
        else
        {
            while (tmp > 0)
            {
                digits[dlen++] = (unsigned char)(tmp % 10);
                tmp /= 10;
            }
        }

        /* Store distinct primes found for this candidate */
        seq_t primes[200];
        size_t pcount = 0;
        bool overflow = false;

        seq_t pow10 = 1;   /* 10^position for little‑endian pos */
        for (size_t pos = 0; pos < dlen && !overflow; ++pos)
        {
            unsigned char old_d = digits[pos];
            unsigned start = 0;
            /* Prevent leading‑zero replacement for the most significant
               digit (pos == dlen-1). */
            if (pos == dlen - 1) start = 1;

            for (unsigned new_d = start; new_d < 10; ++new_d)
            {
                if (new_d == old_d) continue;   /* skip original digit */

                /* New number after replacement */
                seq_t m = cand - (seq_t)old_d * pow10 + (seq_t)new_d * pow10;

                /* Skip the candidate itself and single‑digit results. */
                if (m == cand || m < 10) continue;

                if (!is_prime_big(m)) continue;

                /* Check for duplicates */
                bool seen = false;
                for (size_t k = 0; k < pcount; ++k)
                {
                    if (primes[k] == m) { seen = true; break; }
                }
                if (seen) continue;

                primes[pcount++] = m;
                if (pcount > 8)
                {
                    overflow = true;   /* too many primes; discard candidate */
                    break;
                }
            }
            pow10 *= 10;
        }

        /* Store candidate if it produced exactly 8 distinct primes */
        if (!overflow && pcount == 8)
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp_ptr = mem_resize(seq_t, cache, new_cap);
                if (!tmp_ptr) exit(EXIT_FAILURE);
                cache = tmp_ptr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }

        ++next;
    }

    return cache[n - 1];
}
