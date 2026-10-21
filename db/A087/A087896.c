/*
 * A087896  Least prime p such that n^p - (n-1)^p is composite.
 * https://oeis.org/A087896
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that n^p - (n-1)^p is composite
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 30, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/is_prime_big.h"
#include "../utils/build_primes_upto.h"

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
            seq_t np  = big_pow(n_seq, (seq_t)p);
            seq_t nmp = big_pow(n_minus, (seq_t)p);
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
            seq_t np  = big_pow(n_seq, (seq_t)p);
            seq_t nmp = big_pow(n_minus, (seq_t)p);
            seq_t val = np - nmp;
            if (!is_prime_big(val))
                return (seq_t)p;
        }
        p += 2;
    }
}
