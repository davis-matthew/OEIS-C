/*
 * A095954  Initial values for f(x)=phi(sigma(x)) such that iteration of f
 *          ends in a cycle of length 6.
 * https://oeis.org/A095954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) consists of the integers x for which repeated application
 *            of f(x)=phi(sigma(x)) eventually enters a cycle of length 6. The
 *            implementation tests each positive integer in order, using Floyd
 *            cycle detection to decide whether a number belongs to the
 *            sequence
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*--------------------------------------------------------------*/
/*   Sieve of Eratosthenes – smallest prime factor array        */
/*--------------------------------------------------------------*/
#define SIEVE_MAX 5000000          /* upper bound for SPF array */
static uint32_t spf[SIEVE_MAX + 1];
static uint32_t primes[400000];
static size_t primeCnt = 0;
static bool sieve_done = false;

static void init_sieve(void)
{
    if (sieve_done) return;
    sieve_done = true;
    for (uint32_t i = 2; i <= SIEVE_MAX; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            primes[primeCnt++] = i;
        }
        for (size_t j = 0; j < primeCnt && primes[j] <= spf[i] &&
                 (uint64_t)i * primes[j] <= SIEVE_MAX; ++j)
        {
            spf[i * primes[j]] = primes[j];
        }
    }
}

/*--------------------------------------------------------------*/
/*   Utility calculations : sigma(n) and phi(n)                */
/*--------------------------------------------------------------*/
static uint64_t sigma_calc(uint64_t n)
{
    uint64_t res = 1;
    uint64_t tmp  = n;
    while (tmp > 1) {
        uint32_t p;
        int      exp = 0;

        if (tmp <= SIEVE_MAX && spf[tmp]) {
            p = spf[tmp];
            while (tmp % p == 0) { tmp /= p; ++exp; }
        } else {
            /* trial division for numbers outside the sieve range */
            uint32_t pp = 0;
            for (size_t i = 0; i < primeCnt &&
                 (uint64_t)primes[i] * primes[i] <= tmp; ++i)
            {
                uint32_t q = primes[i];
                if (tmp % q == 0) {
                    p = q;
                    while (tmp % q == 0) { tmp /= q; ++exp; }
                    break;
                }
            }
            if (tmp > 1) {                     /* tmp itself is prime */
                p = (uint32_t)tmp;
                exp = 1;
                tmp = 1;
            }
        }

        __int128 term = 1;
        for (int i = 0; i < exp + 1; ++i) term *= p;
        uint64_t sigma_part = (uint64_t)((term - 1) / (p - 1));
        res *= sigma_part;
    }
    return res;
}

static uint64_t phi_calc(uint64_t n)
{
    uint64_t res = n;
    uint64_t tmp = n;
    for (size_t i = 0; i < primeCnt &&
         (uint64_t)primes[i] * primes[i] <= tmp; ++i)
    {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            res -= res / p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {
        res -= res / tmp;
    }
    return res;
}

/*--------------------------------------------------------------*/
/*   f(n) = phi(sigma(n))                                       */
/*--------------------------------------------------------------*/
static uint64_t compute_f(uint64_t n)
{
    uint64_t sigma = sigma_calc(n);
    return phi_calc(sigma);
}

/*--------------------------------------------------------------*/
/*   Detect whether n eventually reaches a cycle of length 6   */
/*--------------------------------------------------------------*/
static bool has_cycle_len_6(uint64_t start)
{
    uint64_t tortoise = compute_f(start);
    uint64_t hare     = compute_f(compute_f(start));

    /* Floyd's cycle finding */
    int steps = 0;
    const int MAX_STEPS = 500;            /* safety cap */
    while (tortoise != hare) {
        if (steps++ >= MAX_STEPS) return false;   /* give up, not found */
        tortoise = compute_f(tortoise);
        hare     = compute_f(compute_f(hare));
    }

    /* Determine cycle length */
    uint64_t cycle_start = tortoise;
    uint64_t cur = compute_f(cycle_start);
    int len = 1;
    while (cur != cycle_start) {
        cur = compute_f(cur);
        ++len;
        if (len > 6) return false;          /* not a 6‑cycle */
    }
    return len == 6;
}

/*--------------------------------------------------------------*/
/*   Store found terms and memory management                  */
/*--------------------------------------------------------------*/
static seq_t *sequence = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;

static void seq_reserve_extend(void)
{
    if (seq_cap == 0) seq_cap = 64;
    else seq_cap <<= 1;
    seq_t *new_ptr = (seq_t*)realloc(sequence, seq_cap * sizeof(seq_t));
    if (!new_ptr) exit(1);   /* out of memory */
    sequence = new_ptr;
}

static void seq_append(seq_t val)
{
    if (seq_len >= seq_cap) seq_reserve_extend();
    sequence[seq_len++] = val;
}

/*--------------------------------------------------------------*/
/*   Main public function                                      */
/*--------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* safety */
    init_sieve();

    static uint64_t candidate = 1;
    while (seq_len < (size_t)n) {
        if (has_cycle_len_6(candidate)) {
            seq_append((seq_t)candidate);
        }
        ++candidate;
    }
    return sequence[n - 1];
}
