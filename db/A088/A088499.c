/*
 * A088499  Doubly (3)-perfect numbers.
 * https://oeis.org/A088499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A088499
    Doubly (3)-perfect numbers.
    a(n) is the nth integer n such that
    Σ_{d < n, n mod d = 3} d = 2n. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/* ---------- static state ---------- */
static seq_t *seq_buf   = NULL;        /* cached sequence values        */
static size_t seq_cap   = 0;           /* capacity of seq_buf           */
static idx_t  seq_len   = 0;           /* number of values stored       */

static idx_t last_checked_m = 0;      /* last m = n-3 that has been examined */

static idx_t *prime_list  = NULL;     /* list of primes                */
static size_t prime_cnt    = 0;        /* number of primes in list      */
static idx_t  prime_limit  = 0;       /* maximum number sieved         */

/* ---------- helper: ensure primes up to limit ---------- */
static void build_primes(idx_t limit)
{
    if (prime_list != NULL && limit <= prime_limit)
        return;                    /* already sufficient */

    /* free previous allocation if any */
    if (prime_list != NULL) {
        free(prime_list);
        prime_list = NULL;
        prime_cnt  = 0;
    }

    /* simple sieve of Eratosthenes */
    bool *sieve = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!sieve)
        return;                    /* allocation failure unlikely */

    for (idx_t i = 0; i <= limit; ++i)
        sieve[i] = true;
    if (limit >= 0) sieve[0] = false;
    if (limit >= 1) sieve[1] = false;

    idx_t root = (idx_t)sqrt((double)limit);
    for (idx_t i = 2; i <= root; ++i)
        if (sieve[i])
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = false;

    /* count primes */
    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i])
            ++prime_cnt;

    prime_list = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
    idx_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i])
            prime_list[k++] = i;

    prime_limit = limit;
    free(sieve);
}

static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    build_primes(limit);
}

/* ---------- helper: ensure seq buffer capacity ---------- */
static void ensure_seq_capacity(size_t needed)
{
    if (seq_cap >= needed)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_buf = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ---------- helper: compute sigma(m) (sum of divisors of m) ---------- */
static seq_t sigma_of(idx_t m)
{
    if (m == 0)
        return 0;
    seq_t result = 1;
    idx_t temp = m;
    idx_t p;
    for (size_t i = 0; i < prime_cnt && (p = prime_list[i]) * (idx_t)p <= temp; ++i) {
        if (temp % p == 0) {
            unsigned exp = 0;
            seq_t p_pow = 1;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
                p_pow *= (seq_t)p;
            }
            seq_t term = (p_pow * (seq_t)p - 1) / ((seq_t)p - 1);
            result *= term;
        }
    }
    if (temp > 1) {               /* remaining prime factor */
        result *= ((seq_t)temp + 1);
    }
    return result;
}

/* ---------- helper: check if m corresponds to a doubly (3)-perfect n ---------- */
static bool is_doubly3_of(idx_t m)
{
    seq_t sum = sigma_of(m);
    seq_t target = (seq_t)2 * (seq_t)m + 7;
    if ((m & 1) == 0)            /* m even */
        target += 2;
    if (m % 3 == 0)               /* m divisible by 3 */
        target += 3;
    return sum == target;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 0 */

    if (seq_buf == NULL) {
        seq_buf = (seq_t *)malloc(8 * sizeof(seq_t));
        seq_cap = 8;
        seq_len = 0;
    }

    while (seq_len < n) {
        ++last_checked_m;                /* advance m = n-3 */

        /* ensure we have primes up to sqrt(m) */
        idx_t need_prime_limit = (idx_t)sqrt((double)last_checked_m) + 1;
        ensure_primes_upto(need_prime_limit);

        if (is_doubly3_of(last_checked_m)) {
            seq_t n_val = (seq_t)(last_checked_m + 3);
            ensure_seq_capacity(seq_len + 1);
            seq_buf[seq_len++] = n_val;
        }
    }

    return seq_buf[n - 1];
}
