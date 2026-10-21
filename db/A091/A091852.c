/*
 * A091852  Numerator of Sum_{i=1..n} prime(i)/prime(i+1).
 * https://oeis.org/A091852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = numerator of Σ_{i=1..n} prime(i)/prime(i+1) */

/*  Note:  seq_t and idx_t are supplied by the build system.  This
 *  implementation stores the primes in a static array of uint64_t
 *  and converts them to seq_t when used.  The arithmetic on the
 *  numerator and denominator is performed with seq_t, so no overflow
 *  occurs for the arbitrary‑precision build.  The algorithm is O(n)
 *  with a modest constant factor and works for n up to about 10000
 *  quickly in both C11 and C++17.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static uint64_t *prime_arr  = NULL;   /* primes stored as uint64_t */
static size_t   prime_cnt   = 0;     /* number of primes currently in array */
static size_t   prime_cap   = 0;     /* current capacity of the array */

/* Simple primality test using the primes already generated.  */
static bool is_prime_candidate(idx_t cand)
{
    if (cand < 2)
        return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_arr[i];
        if (p * p > cand)
            break;
        if (cand % p == 0)
            return false;
    }
    return true;
}

/* Ensure that prime_cnt >= needed, allocating or enlarging the
 * prime array as necessary.  This function generates primes by simple
 * trial division, which is fast enough for the required range. */
static void ensure_primes(idx_t needed)
{
    if (prime_cnt >= (size_t)needed)
        return;

    if (prime_cap == 0) {
        prime_cap = 16;
        prime_arr = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }

    idx_t cand = (prime_cnt == 0) ? 2
               : (idx_t)(prime_arr[prime_cnt - 1] + 1);

    while (prime_cnt < (size_t)needed) {
        while (!is_prime_candidate(cand))
            ++cand;

        if (prime_cap <= prime_cnt) {
            prime_cap <<= 1;
            uint64_t *tmp = (uint64_t *)realloc(prime_arr,
                                                prime_cap * sizeof(uint64_t));
            prime_arr = tmp;
        }
        prime_arr[prime_cnt++] = (uint64_t)cand;
        ++cand;
    }
}

/* Euclidean algorithm for seq_t.  */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Numerator of Σ_{i=1..n} prime(i)/prime(i+1) for a(1)=2, etc. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* offset is 1; this value never used */

    /* need primes 1..n+1 */
    ensure_primes(n + 1);

    seq_t num = (seq_t)0;
    seq_t den = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t p_i   = (seq_t)prime_arr[i - 1];
        seq_t p_ip1 = (seq_t)prime_arr[i];
        seq_t new_num = num * p_ip1 + den * p_i;
        seq_t new_den = den * p_ip1;
        seq_t g = seq_gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    return num;
}
