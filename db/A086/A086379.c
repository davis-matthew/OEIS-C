/*
 * A086379  Numbers such that both 1 + sum of digits and 1 + product of digits
 *          are prime.
 * https://oeis.org/A086379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A086379: numbers such that 1 + (sum of digits) and 1 + (product of digits)
   are prime.
   The function searches the positive integers in order, tests the two
   primality conditions, stores found values in a static cache, and
   returns the n‑th element (a(1) = 1). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"
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
    /* undefined for n == 0 – the behavior below the OEIS offset is not
       required to be correct, but we guard against it. */
    if (n == 0) {
        return 0;
    }

    /* Static cache of found terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    /* The next integer to test */
    static seq_t next_check  = 1;

    /* Keep finding terms until we have at least n of them. */
    while (cache_len < (size_t)n)
    {
        seq_t m    = next_check;
        seq_t sum  = 0;
        seq_t prod = 1;

        /* If m == 0 (should not happen for n >= 1) handle separately */
        if (m == 0) {
            sum = 0;
            prod = 0;
        } else {
            /* digit extraction in base 10 */
            while (m > 0)
            {
                seq_t digit = m % ((seq_t)10);
                sum += digit;
                prod *= digit;
                m /= ((seq_t)10);
            }
        }

        bool ok = false;
        /* Check 1 + sum of digits is prime, then 1 + product of digits. */
        if (is_prime_big(sum + ((seq_t)1))) {
            ok = is_prime_big(prod + ((seq_t)1));
        }

        if (ok)
        {
            /* Grow cache if necessary */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) {
                    /* Allocation failure – abort the program safely. */
                    /* Avoid a crash in the older C standard; C++ would throw. */
                    exit(1);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_check;
        }

        ++next_check;
    }

    return cache[n - 1];
}
