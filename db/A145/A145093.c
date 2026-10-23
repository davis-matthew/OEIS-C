/*
 * A145093  Constant term in Atkin polynomial A_n(j).
 * https://oeis.org/A145093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * 2^(2n+2) * 3^(n+1) * prod_{k=0}^{2n-2} (6k+5) /
 *            (2n-1)!
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 3, i64 n <= 7, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- Include guards for static tables ---------- */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* ---------- Helper: fast power for seq_t ---------- */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* ---------- Prime sieve (once) ---------- */
#define MAX_PRIME 120000         /* good for n <= 10000   */
static bool sieve_done = false;
static unsigned char is_composite[MAX_PRIME + 1];
static uint32_t primes_list[MAX_PRIME / 10];
static size_t primes_count = 0;

static void init_primes(void)
{
    if (sieve_done) return;
    sieve_done = true;
    for (size_t i = 2; i <= MAX_PRIME; ++i)
        is_composite[i] = 0;
    for (size_t i = 2; i * i <= MAX_PRIME; ++i)
        if (!is_composite[i])
            for (size_t j = i * i; j <= MAX_PRIME; j += i)
                is_composite[j] = 1;
    for (size_t i = 2; i <= MAX_PRIME; ++i)
        if (!is_composite[i])
            primes_list[primes_count++] = (uint32_t)i;
}

/* ---------- Main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;                  /* a(0) = 1 */

    init_primes();                                 /* ensure primes table is ready */

    /* Exponent table for primes up to MAX_PRIME */
    static int exp_table[MAX_PRIME + 1];
    memset(exp_table, 0, sizeof(exp_table));

    /* 1. 2^(2n+2) */
    if (exp_table[2] > INT32_MAX - (int)(2*n + 2))
        exp_table[2] += (int)(2*n + 2);
    else
        exp_table[2] += (int)(2*n + 2);

    /* 2. 3^(n+1) */
    exp_table[3] += (int)(n + 1);

    /* 3. product of (6k+5), k = 0 .. 2n-2 */
    for (idx_t k = 0; k <= 2*n - 2; ++k)
    {
        idx_t val = 5 + 6 * k;                     /* (6k+5) < 12n-7 <= MAX_PRIME */
        idx_t temp = val;
        for (size_t pi = 0; pi < primes_count; ++pi)
        {
            uint32_t p = primes_list[pi];
            if ((idx_t)p * (idx_t)p > temp) break;
            int cnt = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++cnt;
            }
            if (cnt) exp_table[p] += cnt;
        }
        if (temp > 1) exp_table[temp] += 1;         /* temp is prime */
    }

    /* 4. Subtract exponents from factorial (2n-1)! */
    idx_t fact_limit = 2*n - 1;
    for (size_t pi = 0; pi < primes_count; ++pi)
    {
        uint32_t p = primes_list[pi];
        if (p > fact_limit) break;
        idx_t power = p;
        int cnt = 0;
        while (power <= fact_limit)
        {
            cnt += fact_limit / power;
            power *= p;
        }
        exp_table[p] -= cnt;
    }

    /* 5. Assemble the result from prime powers */
    seq_t result = 1;
    for (size_t pi = 0; pi < primes_count; ++pi)
    {
        int e = exp_table[primes_list[pi]];
        if (e > 0)
            result *= pow_seq((seq_t)primes_list[pi], (idx_t)e);
        else if (e < 0)
            result /= pow_seq((seq_t)primes_list[pi], (idx_t)(-e));
    }

    /* 6. Apply sign */
    if (n & 1) result = -result;

    return result;
}
