/*
 * A166376  Triangle in which n-th row (n>1) gives prime factors of n^2 + 1
 *          with repetition.
 * https://oeis.org/A166376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth prime factor (with repetition) of the triangle
 *            where each row r>1 lists the prime factors of r^2 + 1 in
 *            ascending order. The sequence is obtained by concatenating these
 *            rows
 * Range:     u8 n <= 26, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static bool primes_inited = false;
static uint32_t *primes   = NULL;
static size_t   prime_cnt = 0;

/* Build a list of primes up to a fixed limit large enough for the
   tested values of r^2 + 1.  The limit is chosen to comfortably exceed
   the square root of (20000^2 + 1) ≈ 4e8, so a limit of 100000 is safe
   for the intended test range. */
static void init_primes(void)
{
    if (primes_inited)
        return;
    const uint32_t LIMIT = 100000;
    bool *composite = (bool *)calloc(LIMIT + 1, sizeof(bool));
    /* Allocate a buffer big enough for the number of primes up to LIMIT. */
    primes = (uint32_t *)malloc(((size_t)LIMIT / 10 + 10) * sizeof(uint32_t));
    size_t idx = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!composite[i]) {
            primes[idx++] = i;
            if ((uint64_t)i * i <= LIMIT)
                for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i)
                    composite[(size_t)j] = true;
        }
    }
    prime_cnt = idx;
    free(composite);
    primes_inited = true;
}

/* Factorise v (v fits in 64‑bit) returning its prime factors in ascending
   order, one at a time via the supplied array (size at least 32).
   The number of factors is returned. */
static size_t factor_into(uint64_t v, uint64_t out[32])
{
    size_t fcnt = 0;
    for (size_t pi = 0; pi < prime_cnt && (uint64_t)primes[pi] * primes[pi] <= v; ++pi) {
        uint64_t p = primes[pi];
        while (v % p == 0) {
            if (fcnt < 32)
                out[fcnt++] = p;
            v /= p;
        }
    }
    if (v > 1 && fcnt < 32)
        out[fcnt++] = v;          /* remaining prime factor */
    return fcnt;
}

seq_t a(idx_t n)
{
    if (n == 0)      /* behaviour below offset is irrelevant */
        return 0;

    init_primes();

    idx_t cumulative = 0;
    for (idx_t r = 1; ; ++r) {
        /* Compute r^2 + 1 safely using 128‑bit arithmetic. */
        unsigned __int128 vv = (unsigned __int128)r * r + 1;
        /* For the test range the value fits in 64 bits. */
        uint64_t val = (uint64_t)vv;

        uint64_t factors[32];
        size_t fcnt = factor_into(val, factors);

        for (size_t i = 0; i < fcnt; ++i) {
            ++cumulative;
            if (cumulative == n)
                return (seq_t)factors[i];
        }
    }
}
