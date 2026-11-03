/*
 * A198146  Primes remaining primes under map 2<=>9 (interchange of decimal
 *          digits 2 and 9).
 * https://oeis.org/A198146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A198146: the n‑th prime p (p contains both digits 2 and 9) such that
 * swapping every 2 with a 9 and every 9 with a 2 in its decimal
 * representation yields another prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"
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

/* Integer square‑root of a 64‑bit unsigned integer, floor value. */
static idx_t int_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid == 0 || mid > n / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined for n<1, return 0 */

    /* ---------- persistent state ---------- */
    static uint8_t *sieve = NULL;         /* prime sieve up to sieve_limit   */
    static idx_t   sieve_limit = 0;       /* current sieve upper bound      */
    static seq_t  *good  = NULL;          /* collected qualifying primes    */
    static size_t  good_size = 0;         /* number of primes stored        */
    static size_t  good_cap  = 0;         /* capacity of good array         */

    /* ---------- generate terms until we have n of them ---------- */
    while (good_size < (size_t)n) {
        /* determine next sieve limit (start at 1 000 000, then double) */
        idx_t new_limit = (sieve_limit == 0) ? 1000000ULL
                                             : sieve_limit * 2ULL;
        /* avoid overflowing idx_t */
        if (new_limit < sieve_limit) new_limit = UINT64_MAX;

        /* allocate new sieve and copy previous part */
        uint8_t *new_sieve = (uint8_t*)malloc((size_t)new_limit + 1);
        if (!new_sieve) return (seq_t)0;   /* allocation failure */
        if (sieve) {
            memcpy(new_sieve, sieve, (size_t)sieve_limit + 1);
            free(sieve);
        }
        memset(new_sieve, 1, (size_t)new_limit + 1);   /* set all to true */
        new_sieve[0] = 0; new_sieve[1] = 0;             /* 0 and 1 not prime */

        /* Eratosthenes sieve up to new_limit */
        idx_t limit_sqrt = int_sqrt_u64(new_limit);
        for (idx_t p = 2; p <= limit_sqrt; ++p) {
            if (!new_sieve[p]) continue;
            for (idx_t j = p * p; j <= new_limit; j += p)
                new_sieve[j] = 0;
        }

        /* examine primes in the new range */
        idx_t start = (sieve_limit < 2) ? 2 : sieve_limit + 1;
        for (idx_t i = start; i <= new_limit; ++i) {
            if (!new_sieve[i]) continue;          /* i is composite */

            /* extract decimal digits of i */
            uint8_t  digits[20];
            int      len = 0;
            bool     has2 = false, has9 = false;
            uint64_t tmp = i;
            if (tmp == 0) {
                digits[len++] = 0;
            } else {
                while (tmp > 0) {
                    uint8_t d = (uint8_t)(tmp % 10);
                    if (d == 2) has2 = true;
                    else if (d == 9) has9 = true;
                    digits[len++] = d;
                    tmp /= 10;
                }
            }
            if (!has2 || !has9) continue;          /* need both 2 and 9 */

            /* build the 2↔9 swapped number */
            seq_t swapped = 0;
            for (int k = len - 1; k >= 0; --k) {
                uint8_t d = digits[k];
                if (d == 2) d = 9;
                else if (d == 9) d = 2;
                swapped = swapped * 10 + d;
            }

            /* test primality of swapped */
            bool swapped_prime = false;
            if (swapped <= (seq_t)new_limit)
                swapped_prime = new_sieve[(size_t)swapped];
            else
                swapped_prime = is_prime_big(swapped);

            if (!swapped_prime) continue;          /* swapped not prime */

            /* store the qualifying prime */
            if (good_size == good_cap) {
                good_cap = good_cap ? good_cap * 2 : 64;
                seq_t *tmp = mem_resize(seq_t, good, good_cap);
                if (!tmp) return (seq_t)0;          /* allocation fail */
                good = tmp;
            }
            good[good_size++] = (seq_t)i;
        }

        /* commit the new sieve */
        sieve = new_sieve;
        sieve_limit = new_limit;
    }

    return good[n - 1];
}
