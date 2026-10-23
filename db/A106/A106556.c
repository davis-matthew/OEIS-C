/*
 * A106556  Smallest number formed by concatenating the two factors of a
 *          semiprime.
 * https://oeis.org/A106556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest decimal concatenation of the two prime factors
 *            of the nth semiprime. The nth semiprime is the nth integer that
 *            is the product of exactly two primes (not necessarily distinct).
 *            We pre‑compute primes up to a fixed limit and then find
 *            semiprimes on demand by trial division
 * Range:     u8 n <= 10, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   digit_count_uint32, pow10_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_count_uint32.h"
#include "../utils/pow10_exp.h"

#define PRIME_LIMIT 200000     /* enough for >10000 semiprimes          */
#define MAX_SEMI   200000      /* buffer for generated terms            */

static bool primes_ready = false;
static uint32_t primes[PRIME_LIMIT];
static size_t NUM_PRIMES = 0;

/* Generate all primes up to PRIME_LIMIT by a simple sieve. */
static void init_primes(void)
{
    bool *mark = (bool *)malloc(PRIME_LIMIT + 1);
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) mark[i] = true;
    mark[0] = mark[1] = false;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (mark[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                mark[j] = false;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (mark[i]) primes[NUM_PRIMES++] = (uint32_t)i;
    free(mark);
    primes_ready = true;
}

static bool semiprime_ready = false;
static seq_t *values = NULL;      /* array of generated a(n) values      */
static size_t SEMI_COUNT = 0;     /* number of terms already generated   */
static uint64_t next_scan = 4;    /* next integer to test for semiprime  */

/* Return true if num is a semiprime.
   On success, first prime factor is written to *p,
   second factor (may equal first) to *q. */
static bool get_factors(uint64_t num, uint32_t *p, uint32_t *q)
{
    uint32_t first = 0, second = 0;
    size_t multcnt = 0;
    uint64_t tmp = num;

    for (size_t i = 0; i < NUM_PRIMES && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t pr = primes[i];
        if (tmp % pr == 0) {
            if (first == 0) first = pr;
            else if (second == 0) second = pr;
            else return false;               /* more than 2 distinct primes  */
            uint32_t cnt = 0;
            while (tmp % pr == 0) {
                tmp /= pr;
                ++cnt;
            }
            multcnt += cnt;
            if (multcnt > 2) return false;
        }
    }
    if (tmp > 1) {          /* remaining prime factor          */
        if (first == 0) first = (uint32_t)tmp;
        else if (second == 0) second = (uint32_t)tmp;
        else return false;
        ++multcnt;
        if (multcnt > 2) return false;
    }
    if (multcnt != 2) return false;
    *p = first;
    *q = (second == 0) ? first : second;
    return true;
}

/* Ensure values[0..n-1] are generated. */
static void ensure_up_to(idx_t n)
{
    if (!semiprime_ready) {
        values = (seq_t *)malloc(MAX_SEMI * sizeof(seq_t));
        SEMI_COUNT = 0;
        next_scan = 4;
        semiprime_ready = true;
    }
    while (SEMI_COUNT < (size_t)n) {
        uint32_t p, q;
        while (!get_factors(next_scan, &p, &q)) ++next_scan;  /* find next semiprime */

        size_t d1 = digit_count_uint32(p);
        size_t d2 = digit_count_uint32(q);
        seq_t a1 = (seq_t)p * pow10_exp((uint32_t)d2) + q;
        seq_t a2 = (seq_t)q * pow10_exp((uint32_t)d1) + p;
        values[SEMI_COUNT++] = (a1 < a2 ? a1 : a2);
        ++next_scan;
    }
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (!primes_ready) init_primes();
    ensure_up_to(n);
    return values[n - 1];      /* n is 1‑based */
}
