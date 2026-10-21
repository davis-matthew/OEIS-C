/*
 * A079616  Prime factorization templates.
 * https://oeis.org/A079616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A077462(n) - 1 (rank of the prime‑factorisation template
 *            of n minus 1)
 * Range:     u8 n <= 8929, u16 n <= 8929, u32 n <= 8929, u64 n <= 8929,
 *            u128 n <= 8929, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"

#define PRIMELIMIT   100000   /* upper bound for sieve */
#define MAXPRIMES    10000    /* number of primes that will be stored */

static bool primes_init = false;
static int  prime_cnt   = 0;
static uint32_t primes[MAXPRIMES];

/* generate the list of primes once, used by the count routine */
static void init_primes(void)
{
    if (primes_init) return;

    static bool marked[PRIMELIMIT + 1];
    /* marked array is zero‑initialised automatically */

    for (int i = 2; i <= PRIMELIMIT; ++i) {
        if (!marked[i]) {
            if (prime_cnt < MAXPRIMES) primes[prime_cnt++] = i;
            if ((int64_t)i * i <= PRIMELIMIT) {
                for (int j = i * i; j <= PRIMELIMIT; j += i)
                    marked[j] = true;
            }
        }
    }
    primes_init = true;
}

/* Count the number of distinct templates whose minimal value does not exceed
   'limit'.  Templates are produced recursively; each level chooses an
   exponent for the current prime and then recurses on the remaining primes. */
static seq_t count_le(seq_t limit, int idx)
{
    if (idx >= prime_cnt) return 0;

    seq_t total = 0;
    seq_t p     = (seq_t)primes[idx];
    seq_t power = p;

    while (power <= limit) {
        /* pattern that ends with this prime having the current exponent */
        ++total;

        /* further primes may follow */
        seq_t new_limit = limit / power;
        total += count_le(new_limit, idx + 1);

        /* next exponent for the current prime */
        if (power > limit / p) break;  /* would overflow or exceed the limit */
        power *= p;
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;   /* empty template */

    init_primes();

    /* factorise n, storing exponents in the order of the primes */
    idx_t tmp = n;
    int exps[64];
    int k = 0;

    for (int i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        if (tmp % primes[i] == 0) {
            int e = 0;
            while (tmp % primes[i] == 0) {
                tmp /= primes[i];
                ++e;
            }
            exps[k++] = e;
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        exps[k++] = 1;
    }

    /* compute the minimal number for this template:
       multiply primes[i]^exps[i] using the first k primes */
    seq_t min_val = 1;
    for (int i = 0; i < k; ++i) {
        seq_t part = pow_seq((seq_t)primes[i], exps[i]);
        min_val *= part;
    }

    /* rank = 1 (empty template) + number of templates with minimal value ≤ min_val */
    seq_t rank = (seq_t)1 + count_le(min_val, 0);

    return rank - (seq_t)1;
}
