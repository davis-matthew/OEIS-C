/*
 * A090725  Primes whose representation in base 16 has no alphabetic
 *          characters.
 * https://oeis.org/A090725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 916 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by constructing the list of primes ≤ LIMIT whose
   hexadecimal representation contains only decimal digits (0–9).  The
   nth element of that list is returned.  EOF */  

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Build all qualifying primes up to 'limit' and store them in *out.
   The array contains uint64_t values that can be cast to seq_t.          */
static void build_hex_primes(idx_t limit, uint64_t **out, size_t *cnt)
{
    /* Sieve of Eratosthenes ------------------------------------------- */
    uint8_t *comp = (uint8_t *)malloc((size_t)(limit + 1));
    if (!comp) return;                       /* allocation failed */
    memset(comp, 0, (size_t)(limit + 1));
    comp[0] = comp[1] = 1;

    for (idx_t i = 2; i * i <= limit; ++i)
    {
        if (!comp[i])
        {
            for (idx_t j = i * i; j <= limit; j += i)
                comp[j] = 1;
        }
    }

    /* Collect primes whose hex digits are all ≤ 9 ----------------------- */
    size_t cap = 64;
    uint64_t *buf = (uint64_t *)malloc(cap * sizeof(uint64_t));
    size_t sz = 0;

    for (idx_t i = 2; i <= limit; ++i)
    {
        if (!comp[i])
        {
            uint64_t tmp = (uint64_t)i;
            bool ok = true;
            while (tmp)
            {
                uint64_t d = tmp & 0xFull;   /* lowest hex digit */
                if (d > 9) { ok = false; break; }
                tmp >>= 4;
            }
            if (ok)
            {
                if (sz == cap)
                {
                    cap <<= 1;
                    uint64_t *newbuf = (uint64_t *)realloc(buf, cap * sizeof(uint64_t));
                    if (!newbuf) { free(buf); free(comp); return; }
                    buf = newbuf;
                }
                buf[sz++] = i;
            }
        }
    }

    free(comp);
    *out = buf;
    *cnt = sz;
}

/* The main sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    /* Limit chosen to exceed the required n = 10 000 (about 48 000 terms are
       available up to 20 000 000).                                            */
    static const idx_t LIMIT = 20000000ULL;
    static uint64_t *hex_primes = NULL;
    static size_t hex_len = 0;
    static bool initialized = false;

    if (!initialized)
    {
        build_hex_primes(LIMIT, &hex_primes, &hex_len);
        initialized = true;
    }

    if (n == 0 || n > hex_len || hex_primes == NULL) return (seq_t)0;
    return (seq_t)hex_primes[(size_t)(n - 1)];
}
