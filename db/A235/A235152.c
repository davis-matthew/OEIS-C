/*
 * A235152  Let x(1)x(2)...x(q) the decimal expansion of the numbers k having
 *          exactly q distinct prime divisors p(1) < p(2) < ... < p(q).
 *          Sequence lists the numbers k such that p(1)/x(1) + p(2)/x(2) + ...
 *          + p(q)/x(q) is an integer.
 * https://oeis.org/A235152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A235152: numbers k such that
   for k with distinct primes p(1)<…<p(q), the sum
   p(1)/x(1)+…+p(q)/x(q) is an integer,
   where x(1)…x(q) are the first q decimal digits of k.
   We also require that the number of digits of k equals q
   (this excludes primes with more than one digit, e.g. 11). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* utilities provided by the harness */
#include "../utils/digit_count.h"

/* --- auxiliary utilities ------------------------------------------------ */

static inline uint32_t gcd32(uint32_t a, uint32_t b)
{
    while (b) {
        uint32_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* --- main function -------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* 1. Constants --------------------------------------------------- */
    const idx_t  TARGET   = 20000;           /* enough for n up to 10000 */
    const idx_t  MAX_LIMIT = 20000000U;      /* search limit for k        */

    /* 2. static state -------------------------------------------------- */
    static seq_t *seq   = NULL;
    static idx_t  seq_len  = 0;
    static idx_t  seq_cap  = 0;

    static uint32_t *spf = NULL;     /* smallest prime factor array */
    static idx_t  processed = 0;      /* last processed integer      */

    /* 3. first call: initialise ------------------------------------------------ */
    if (seq_len == 0) {
        /* allocate sequence array */
        seq_cap = TARGET;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) return 0;                          /* unlikely   */

        /* allocate spf array */
        spf = (uint32_t *)calloc(MAX_LIMIT + 1U, sizeof(uint32_t));
        if (!spf) {
            free(seq); seq = NULL; return 0;
        }

        /* classic Eratosthenes sieve that also records the smallest prime
           factor for every integer up to MAX_LIMIT.  For primes p we set
           spf[p] = p.  For composites, the first time we visit a multiple
           we store the smallest prime factor encountered. */
        for (idx_t i = 2U; i <= MAX_LIMIT; ++i) {
            if (spf[i] == 0U) {                /* i is prime */
                spf[i] = (uint32_t)i;
                if ((uint64_t)i * i <= MAX_LIMIT) {
                    for (uint64_t j = (uint64_t)i * i;
                         j <= MAX_LIMIT; j += i) {
                        if (spf[(size_t)j] == 0U)
                            spf[(size_t)j] = (uint32_t)i;
                    }
                }
            }
        }

        /* 4. scan integers and collect qualifying ones ------------------- */
        processed = 1U;   /* start checking from 2 */
        while (processed < MAX_LIMIT && seq_len < TARGET) {
            ++processed;
            uint64_t k = processed;

            /* factorise k into distinct primes */
            uint64_t primes[16];
            size_t   pcnt = 0;
            uint64_t tmp = k;
            while (tmp > 1U && pcnt < 16U) {
                uint64_t p = spf[tmp];
                if (p == 0U) p = tmp;          /* should not happen for <MAX_LIMIT */
                primes[pcnt++] = p;
                while (tmp % p == 0U)
                    tmp /= p;
            }
            if (tmp > 1U) {                     /* remainder prime > limit?  */
                primes[pcnt++] = tmp;
            }

            const size_t q = pcnt;              /* number of distinct primes */

            /* require number of digits to equal q */
            uint64_t d = digit_count(k);
            if (d != q)
                continue;

            /* extract the first q (= d) digits of k */
            uint8_t digits[20];
            uint64_t tt = k;
            for (int pos = (int)q - 1; pos >= 0; --pos) {
                digits[pos] = (uint8_t)(tt % 10U);
                tt /= 10U;
            }

            /* zero digits invalidate the sum */
            bool has_zero = false;
            for (size_t j = 0; j < q; ++j) {
                if (digits[j] == 0U) { has_zero = true; break; }
            }
            if (has_zero)
                continue;

            /* compute lcm of the digits (they are 1..9) */
            uint32_t lcm = 1U;
            for (size_t j = 0; j < q; ++j) {
                uint32_t g = gcd32(lcm, (uint32_t)digits[j]);
                lcm = (lcm / g) * (uint32_t)digits[j];
            }

            /* compute numerator sum */
            uint64_t numer = 0U;
            for (size_t j = 0; j < q; ++j) {
                numer += primes[j] * ((uint64_t)lcm / (uint64_t)digits[j]);
            }

            if (numer % lcm == 0U) { /* integer condition satisfied */
                if (seq_len >= seq_cap) {
                    seq_cap *= 2;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                    if (!seq) return 0; /* allocation failure */
                }
                seq[seq_len++] = (seq_t)k;
            }
        }

        /* clean up SPF memory – it is no longer needed */
        free(spf);
        spf = NULL;
    }

    /* 5. answer query ----------------------------------------------- */
    if (n == 0 || n > seq_len)
        return 0;      /* undefined (should not happen for proper n) */

    return seq[n - 1U];
}
