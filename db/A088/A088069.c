/*
 * A088069  Cubes sandwiched between two numbers divisible by cubes > 1.
 * https://oeis.org/A088069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088069:  a(n) = k³ where k³−1 and k³+1 each have a divisor
   that is a perfect cube > 1.  The function determines successive
   suitable k by brute‑force testing of the two numbers.  Prime tables
   are built once, and factorisation checks stop at the cube root of
   the tested value.  This satisfies the required time bound for
   n up to about 10 000. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "../utils/floor_cube_root.h"   /* floor_cube_root() for seq_t */

/* maximum prime needed: cube root of (k³+1) for the largest k to be
   searched.  k up to ~10⁶ gives k³≈10¹⁸, cube root ≈10⁶.  */
#define MAX_PRIME 1000000U

static uint8_t sieve[MAX_PRIME + 1];
static uint32_t primes[MAX_PRIME + 1];
static size_t   primes_cnt = 0;

/* generate primes up to MAX_PRIME once */
static void init_primes(void)
{
    if (primes_cnt > 0) return;          /* already initialised */

    memset(sieve, 1, sizeof(sieve));
    sieve[0] = sieve[1] = 0;

    for (uint32_t i = 2; i <= MAX_PRIME; ++i) {
        if (sieve[i]) {
            primes[primes_cnt++] = i;
            if ( (uint64_t)i * i <= MAX_PRIME ) {
                for (uint32_t j = i * i; j <= MAX_PRIME; j += i)
                    sieve[j] = 0;
            }
        }
    }
}

/* return true if m has a divisor d>1 with d³ | m                       */
static bool has_cube_factor(seq_t m)
{
    if (m <= 1) return false;

    seq_t temp = m;
    seq_t cube_limit = floor_cube_root(temp);   /* integer cube root */

    for (size_t idx = 0; idx < primes_cnt && (seq_t)primes[idx] <= cube_limit; ++idx) {
        uint32_t p = primes[idx];
        seq_t p_cube = (seq_t)p * p * p;
        if (p_cube > temp) break;

        int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
            if (exp >= 3) return true;
        }
        if (temp == 1) break;          /* fully factored */
    }

    /* if remainder > 1, it might be a product of primes all with exponent <3.
       A remaining prime factor > cube_limit automatically has exponent 1, so
       cannot contribute a cube divisor.  Thus we can safely return false.   */
    return false;
}

/* main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined below offset */

    init_primes();

    idx_t  found = 0;
    idx_t  k = 2;                            /* k ≥ 2 because 1³=1 gives 0,2 not cubes */

    while (found < n) {
        /* compute k³, k³-1, k³+1 in seq_t */
        seq_t ksq  = (seq_t)k * k;
        seq_t kcub = ksq * k;

        seq_t low  = kcub - 1;
        seq_t high = kcub + 1;

        if (has_cube_factor(low) && has_cube_factor(high)) {
            ++found;
            if (found == n)
                return kcub;                /* k³ is the wanted term */
        }
        ++k;
    }

    return (seq_t)0;                         /* never reached */
}
