/*
 * A181715  Length of the complete Cunningham chain of the second kind
 *          starting with prime(n).
 * https://oeis.org/A181715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 4239, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 9916 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n): length of the Cunningham chain of the second kind starting
 * with the n-th prime.  Compute the number of iterations of 
 * x -> 2*x-1 needed to reach a composite number.
 *
 * Uses a precomputed list of primes sufficient for all n <= 10000.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
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

/* pre‑computed primes up to 2,000,000 (∼1.5×10^5 primes)  */
static uint32_t prime_list[200000];
static size_t prime_cnt = 0;
static bool sieve_done   = false;

static void init_primes(void)
{
    if (sieve_done) return;

    const size_t limit = 2000000;
    bool *is_comp = (bool *)calloc(limit + 1, sizeof(bool)); /* false = prime */
    for (size_t i = 2; i * i <= limit; ++i)
        if (!is_comp[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i])
            prime_list[idx++] = (uint32_t)i;
    free(is_comp);

    prime_cnt = idx;
    sieve_done = true;
}

seq_t a(idx_t n)
{
    init_primes();                 /* ensure primes are ready */

    if (n < 1) return 0;           /* undefined below offset */
    if (n > (idx_t)prime_cnt) return 0;  /* safety check */

    seq_t cur   = (seq_t)prime_list[(size_t)(n - 1)];
    seq_t steps = 0;

    while (true) {
        seq_t nxt = (seq_t)2 * cur - (seq_t)1;
        ++steps;
        if (!is_prime_big(nxt))
            break;
        cur = nxt;
    }
    return steps;
}
