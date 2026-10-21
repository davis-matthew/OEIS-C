/*
 * A059592  Square-full part of n^2 + 1.
 * https://oeis.org/A059592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A000188(A002522(n)) (square‑root of the largest perfect
 *            square dividing n^2+1)
 * Range:     u8 n <= 17, u16 n <= 331, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static int *prime_list = NULL;
static size_t prime_count = 0;

/* simple sieve of Eratosthenes up to limit */
static void init_primes(void)
{
    if (prime_list != NULL)
        return; /* already initialized */

    const size_t LIMIT = 100000; /* sufficient for sqrt(n^2+1) when n <= 10000 */
    bool *is_prime = (bool *)malloc(LIMIT + 1);
    for (size_t i = 0; i <= LIMIT; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;

    prime_list = (int *)malloc(LIMIT * sizeof(int));
    prime_count = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            prime_list[prime_count++] = (int)i;

    free(is_prime);
}

/* square‑root of the largest perfect square dividing n^2+1 */
seq_t a(idx_t n)
{
    init_primes();

    /* compute s = n^2 + 1 */
    seq_t s = (seq_t)n * (seq_t)n + 1;

    seq_t res = 1;

    for (size_t i = 0; i < prime_count; ++i) {
        const seq_t p = (seq_t)prime_list[i];
        if (p * p > s)
            break;
        int exp = 0;
        while (s % p == 0) {
            s /= p;
            ++exp;
        }
        /* floor(exp/2) times multiply p */
        for (int k = 0; k < exp / 2; ++k)
            res *= p;
    }
    /* remaining s is either 1 or a prime with exponent 1 -> floor(1/2)=0, ignore */

    return res;
}
