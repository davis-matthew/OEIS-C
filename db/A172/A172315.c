/*
 * A172315  Primes of the form 2^i*3^j - 1 with i + j = 13.
 * https://oeis.org/A172315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: full, pass, 7 terms through n = 7 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A172315: Primes of the form 2^i * 3^j - 1 with i + j = 13. */
/* The function generates all candidates, tests primality with the provided helper
   is_prime_big(), stores the primes in increasing order, and returns the nth prime
   (1‑based).  The 14 candidates are generated only once; subsequent calls reuse the
   cached array. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* All possible exponents (i, j) with i + j = 13 */
    const idx_t EXPO = 13U;

    /* Static cache of found primes.  There are exactly 7 such primes. */
    static seq_t primes[7];
    static idx_t prime_count = 0;
    static bool initialized = false;

    if (!initialized)
    {
        /* Pre‑compute powers of 3 up to 3^13 in a temporary 64‑bit array to avoid
           overflow of the target type when seq_t is smaller.  We will cast the
           result into seq_t after the multiplication with 2^i. */
        idx_t k;
        uint64_t pow3[EXPO + 1];
        pow3[0] = 1ULL;
        for (k = 1; k <= EXPO; ++k)
            pow3[k] = pow3[k - 1] * 3ULL;

        /* Enumerate all 14 (i, j) pairs. */
        for (k = 0; k <= EXPO; ++k)
        {
            idx_t i = k;
            idx_t j = EXPO - k;

            /* Compute 2^i * 3^j - 1.  Use 64‑bit arithmetic for safety,
               then cast to seq_t. */
            uint64_t pow2_i = ((uint64_t)1) << i;             /* 2^i  */
            uint64_t candidate64 = pow2_i * pow3[j];          /* 2^i * 3^j  */
            candidate64 -= 1ULL;                              /* subtract 1  */
            seq_t candidate = (seq_t)candidate64;

            if (is_prime_big(candidate))
            {
                primes[prime_count++] = candidate;
            }
        }

        /* Sort the primes in increasing order (simple insertion sort). */
        for (k = 0; k < prime_count; ++k)
        {
            for (idx_t m = k + 1; m < prime_count; ++m)
            {
                if (primes[k] > primes[m])
                {
                    seq_t tmp = primes[k];
                    primes[k] = primes[m];
                    primes[m] = tmp;
                }
            }
        }

        initialized = true;
    }

    /* Return the requested term (1‑based).  Return 0 if n is out of range. */
    if (n == 0 || n > prime_count)
        return (seq_t)0;
    return primes[n - 1];
}
