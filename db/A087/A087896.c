/*
 * A087896  Least prime p such that n^p - (n-1)^p is composite.
 * https://oeis.org/A087896
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that n^p - (n-1)^p is composite
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9942 (2026-11-03)
 * Helpers:   build_primes_upto, ipow, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow.h"
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

static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void ensure_primes(void)
{
    if (!primes_ready)
    {
        size_t cnt = 0;
        uint64_t *p = NULL;
        /* Primes up to 10000 are more than enough for n ≤ 10 000 */
        build_primes_upto((idx_t)10000, &p, &cnt);
        primes     = p;
        prime_cnt  = cnt;
        primes_ready = true;
    }
}

seq_t a(idx_t n)
{
    if (n < 2)       /* undefined for n < 2; return 0 as placeholder */
        return (seq_t)0;

    ensure_primes();

    seq_t n_seq   = (seq_t)n;
    seq_t n_minus = (seq_t)(n - 1);

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        idx_t p = (idx_t)primes[i];
        seq_t val;

        if (p == 2)
            val = 2 * n_seq - 1;          /* 2n-1 */
        else if (p == 3)
            val = 3 * n_seq * n_seq
                  - 3 * n_seq + 1;       /* 3n²-3n+1 */
        else
        {
            seq_t np  = ipow(n_seq, (seq_t)p);
            seq_t nmp = ipow(n_minus, (seq_t)p);
            val = np - nmp;
        }

        if (!is_prime_big(val))
            return (seq_t)p;
    }

    /* Fallback: in the unlikely event the answer exceeds our prime table */
    idx_t p = primes[prime_cnt - 1] + 2;
    for (;;)
    {
        if (is_prime_big((seq_t)p))
        {
            seq_t np  = ipow(n_seq, (seq_t)p);
            seq_t nmp = ipow(n_minus, (seq_t)p);
            seq_t val = np - nmp;
            if (!is_prime_big(val))
                return (seq_t)p;
        }
        p += 2;
    }
}
