/*
 * A092772  (Prime(prime(n))^2-1)/24.
 * https://oeis.org/A092772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(prime(n+1))^2 - 1) / 24
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset : 1   (a(1) corresponds to the first listed term) */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* ----------------------------------------------------------- */
/*  A simple fixed‑size sieve – 2 000 000 is enough for n ≲ 100 000 */
/* ----------------------------------------------------------- */
#define SIEVE_LIMIT   2000000      /* Upper bound for prime values   */
#define PRIME_CAP     150000       /* Enough to hold all primes <= SIEVE_LIMIT */

static bool          sieve_done = false;
static bool          composite[SIEVE_LIMIT + 1];
static idx_t         primes[PRIME_CAP];
static size_t        prime_cnt  = 0;

/* Build the prime list once */
static void init_sieve(void)
{
    if (sieve_done) return;

    memset(composite, 0, sizeof(composite));

    size_t cnt = 0;
    for (int i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!composite[i]) {
            if (cnt < PRIME_CAP)
                primes[cnt++] = (idx_t)i;
            if ((int64_t)i * i <= SIEVE_LIMIT) {
                for (int j = i * i; j <= SIEVE_LIMIT; j += i)
                    composite[j] = true;
            }
        }
    }
    prime_cnt = cnt;
    sieve_done = true;
}

/* ----------------------------------------------------------- */
/*  Sequence function                                            */
/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise sieve on first call */
    init_sieve();

    if (n == 0) return 0;               /* undefined; safe return */

    /* (n+1)‑th prime – offset because primes[0] = 2 */
    if (n + 1 > prime_cnt) return 0;    /* safety check */
    idx_t p1 = primes[(size_t)(n)];     /* p_{n+1} */

    /* Ensure we have enough primes to index p1 */
    if (p1 > prime_cnt) return 0;       /* should not happen with current limit */

    /* prime of that prime: p_{p_{n+1}} */
    idx_t p2 = primes[(size_t)(p1 - 1)];

    /* Compute (p2^2 - 1)/24 using a wider intermediate if available */
    unsigned __int128 sq = (unsigned __int128)p2 * (unsigned __int128)p2;
    seq_t   val = (seq_t)((sq - 1) / 24);
    return val;
}
