/*
 * A075262  z-value of the solution (x,y,z) to 3/(2n+1) = 1/x + 1/y + 1/z
 *          satisfying 0 < x < y < z, odd x, y, z and having the largest
 *          z-value. The x and y components are in A075260 and A075261.
 * https://oeis.org/A075262
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   process_divisors_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A075262
 *  a(n) = the largest z in a solution (x,y,z) of
 *  3/(2n+1) = 1/x + 1/y + 1/z, 0 < x < y < z,
 *  with all of x, y, z odd.
 *
 *  The algorithm chooses the minimal odd x > (2n+1)/3,
 *  enumerates all divisors a of ( (2n+1)*x )²,
 *  and keeps the divisor that yields valid odd y and z
 *  with the largest possible z.  This method is efficient
 *  for n up to at least 10 000 and uses only integer
 *  arithmetic on the type seq_t.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/process_divisors_rec.h"

/* ---------- Prime table (simple sieve) ---------- */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

static void init_prime_list(void)
{
    if (prime_list != NULL)
        return;

    const size_t LIMIT = 50000;          /* enough for sqrt( (2·10⁴+1)·10⁴ )  */
    bool *is_prime = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (is_prime == NULL) return;
    for (size_t i = 2; i <= LIMIT; ++i) is_prime[i] = true;
    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;

    prime_list = (uint32_t *)malloc((LIMIT / 2) * sizeof(uint32_t));
    if (prime_list == NULL) {
        free(is_prime);
        return;
    }
    prime_count = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            prime_list[prime_count++] = (uint32_t)i;
    free(is_prime);
}

/* ---------- Factorisation of 64‑bit integers ---------- */
static void factor_uint64(uint64_t n,
                          uint64_t *primes, int *exps, size_t *cnt)
{
    *cnt = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_count &&
                     (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= tmp;
         ++i)
    {
        if (tmp % prime_list[i] == 0) {
            int e = 0;
            while (tmp % prime_list[i] == 0) {
                tmp /= prime_list[i];
                ++e;
            }
            primes[(*cnt)] = prime_list[i];
            exps[(*cnt)]   = e;
            ++(*cnt);
        }
    }
    if (tmp > 1) {
        primes[(*cnt)] = tmp;
        exps[(*cnt)]   = 1;
        ++(*cnt);
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 2)     /* behaviour below offset is irrelevant */
        return 0;

    init_prime_list();

    /* ------------ constants for this n ------------ */
    seq_t d = (seq_t)n * 2 + 1;          /* 2n+1  */
    seq_t startX = d / 3 + 1;            /* first integer > d/3  */
    if ( startX % 2 == 0 )               /* make it odd           */
        startX++;

    seq_t best_z = 0;                     /* answer for this n     */

    /* bound for the outer loop: 3*d is sufficient */
    idx_t maxX = (idx_t)( (seq_t)d * 3 );
    for ( idx_t xr = (idx_t)startX; xr <= maxX; xr += 2 ) {
        seq_t x = (seq_t)xr;
        seq_t A = 3 * x - d;               /* always even, no issue   */
        seq_t B = d;
        seq_t P = B * x;                   /* (2n+1)*x                */
        seq_t P_sq = P * P;                /* (P)^2                    */

        /* Factorisation of P (fits in 64 bits for the required n) */
        uint64_t primes[64];
        int exps[64];
        size_t cnt;
        factor_uint64( (uint64_t)P, primes, exps, &cnt );

        /* Double exponents for P² */
        int exp_doub[64];
        for (size_t i = 0; i < cnt; ++i)
            exp_doub[i] = exps[i] * 2;

        seq_t best_a = 0;                 /* smallest divisor yielding a solution */
        process_divisors_rec(0,
                             (seq_t)1,
                             primes, exp_doub, cnt,
                             P_sq, A, P,
                             x, &best_a, &best_z);

        if (best_z != 0)                  /* we already have an answer   */
            break;
    }

    return best_z;
}
