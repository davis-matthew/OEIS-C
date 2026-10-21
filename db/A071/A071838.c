/*
 * A071838  a(n) = Pi(8,3)(n) + Pi(8,5)(n) - Pi(8,1)(n) - Pi(8,7)(n) where
 *          Pi(a,b)(x) denotes the number of primes in the arithmetic
 *          progression a*k + b less than or equal to x.
 * https://oeis.org/A071838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Pi(8,3)(n) + Pi(8,5)(n) – Pi(8,1)(n) – Pi(8,7)(n) (number
 *            of odd primes ≤ n with 2 a quadratic non‑residue minus those
 *            with 2 a quadratic residue)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* --- static cache for a simple sieve --------------------------------- */
static uint8_t  *is_prime   = NULL;   /* 0/1 flag array for 0..max_limit       */
static uint64_t *prime_list = NULL;   /* list of primes up to max_limit       */
static size_t    prime_cnt  = 0;      /* number of stored primes              */
static idx_t     max_limit  = 0;      /* largest number already sieved        */

/* rebuild the sieve and prime list up to ‘limit’ (inclusive) */
static void rebuild_sieve(idx_t limit)
{
    if (limit <= max_limit)
        return;                      /* already sufficient                */

    size_t l = (size_t)limit;             /* safe cast for array sizes */

    /* enlarge is_prime table */
    uint8_t *tmp = (uint8_t*)realloc(is_prime, l + 1);
    if (!tmp) exit(EXIT_FAILURE);
    is_prime = tmp;
    for (size_t i = 0; i <= l; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;        /* 0 and 1 are not prime */

    /* simple Eratosthenes sieve */
    for (size_t i = 2; i * i <= l; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= l; j += i)
                is_prime[j] = 0;

    /* count primes to resize prime_list */
    size_t count = 0;
    for (size_t i = 2; i <= l; ++i)
        if (is_prime[i])
            ++count;

    uint64_t *tmp2 = (uint64_t*)realloc(prime_list, count * sizeof(uint64_t));
    if (!tmp2) exit(EXIT_FAILURE);
    prime_list = tmp2;

    size_t idx = 0;
    for (size_t i = 2; i <= l; ++i)
        if (is_prime[i])
            prime_list[idx++] = i;

    prime_cnt = count;
    max_limit = l;
}

/* --- sequence function ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2)                     /* no odd primes ≤ 1 */
        return (seq_t)0;

    rebuild_sieve(n);               /* ensure data up to n */

    seq_t sum = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > n)
            break;                  /* primes list sorted */

        uint8_t r = p & 7;          /* p mod 8 */
        if (r == 1 || r == 7)
            sum -= 1;               /* 2 is a quadratic residue */
        else if (r == 3 || r == 5)
            sum += 1;               /* 2 is a quadratic non‑residue */
    }
    return sum;
}
