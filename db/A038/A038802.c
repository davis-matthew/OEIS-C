/*
 * A038802  Factor 2n+1 = (2^m1)*(3^m2)*(5^m3)*...; a(n) = number of initial
 *          zero exponents.
 * https://oeis.org/A038802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of consecutive primes < the smallest prime divisor
 *            of 2n+1, i.e., the index (0‑based) of that divisor among the
 *            primes 2,3,5,7,11,…
 * Range:     u8 n <= 137, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper to ensure a prime sieve up to at least 'limit'.  prime_flag[i]==1 iff i is prime. */
static uint8_t *prime_flag = NULL;
static seq_t prime_flag_limit = 0;

static void ensure_prime_flag(seq_t limit)
{
    if (limit <= prime_flag_limit)
        return;
    /* Allocate new array and build sieve from scratch up to 'limit' */
    size_t sz = (size_t)(limit + 1);
    uint8_t *new_flag = (uint8_t *)malloc(sz);
    if (!new_flag)
        return; /* allocation failure – unlikely for the small limits used */

    for (size_t i = 0; i <= limit; ++i)
        new_flag[i] = 1;
    if (limit >= 0) new_flag[0] = 0;
    if (limit >= 1) new_flag[1] = 0;

    for (seq_t i = 2; i * i <= limit; ++i) {
        if (new_flag[(size_t)i]) {
            for (seq_t j = i * i; j <= limit; j += i)
                new_flag[(size_t)j] = 0;
        }
    }

    free(prime_flag);
    prime_flag = new_flag;
    prime_flag_limit = limit;
}

/* Main: a(idx_t n) */
seq_t a(idx_t n)
{
    /* 2n+1 is always odd and >= 3 for n >= 1. */
    seq_t x = ((seq_t)n) * 2 + 1;

    /* Find the smallest prime divisor of x (x is odd, so start at 3). */
    seq_t smallest = x;          /* default: assume x is prime */
    for (seq_t d = 3; d <= x / d; d += 2) {
        if (x % d == 0) {
            smallest = d;
            break;
        }
    }

    /* Count how many primes are strictly less than 'smallest'. */
    if (smallest <= 2)          /* x cannot be even, thus this case cannot happen */
        return 0;

    /* Prepare prime table up to at least smallest-1. */
    ensure_prime_flag(smallest - 1);

    seq_t count = 0;
    for (seq_t p = 2; p < smallest; ++p)
        if (prime_flag[(size_t)p])
            ++count;

    return count;
}
