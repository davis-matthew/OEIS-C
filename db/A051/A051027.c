/*
 * A051027  a(n) = sigma(sigma(n)) = sum of the divisors of the sum of the
 *          divisors of n.
 * https://oeis.org/A051027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, look
 * Formula:   a(n) = sigma(sigma(n))
 * Range:     u8 n <= 53, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* ---------- prime table (sieved once) ---------- */
#define PRIME_LIMIT 20000
static unsigned char prime_flag[PRIME_LIMIT + 1];
static unsigned int primes[PRIME_LIMIT];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt) return;                     /* already initialized */
    for (int i = 2; i <= PRIME_LIMIT; ++i) prime_flag[i] = 0;
    for (int i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!prime_flag[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                prime_flag[j] = 1;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (!prime_flag[i]) primes[prime_cnt++] = (unsigned int)i;
}

/* ---------- divisor sum sigma(n) via prime factorization ---------- */
static seq_t sigma_of(idx_t n)
{
    if (n <= 1) return (seq_t)1;      /* sigma(1) = 1 */
    init_primes();
    seq_t result = 1;
    idx_t m = n;

    for (size_t i = 0; i < prime_cnt; ++i) {
        unsigned int p = primes[i];
        seq_t p_sq = (seq_t)p * (seq_t)p;
        if (p_sq > (seq_t)m) break;
        if (m % p == 0) {
            seq_t term = 1;           /* sum_{k=0..e} p^k */
            seq_t p_pow = 1;
            while (m % p == 0) {
                m /= p;
                p_pow *= (seq_t)p;    /* p^k */
                term += p_pow;       /* add to sum */
            }
            result *= term;
        }
    }
    if (m > 1)                      /* remaining prime factor */
        result *= ((seq_t)1 + (seq_t)m);
    return result;
}

/* ---------- main function required by the spec ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* out of bounds */
    seq_t s1 = sigma_of(n);
    seq_t s2 = sigma_of((idx_t)s1);
    return s2;
}
