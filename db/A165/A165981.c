/*
 * A165981  Primes p which are equal to 2 plus the sum of three consecutive
 *          primes.
 * https://oeis.org/A165981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 396, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 958 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Primes p that are equal to 2 plus the sum of three consecutive primes
    a(n) = the n-th prime p for which there exist consecutive primes
           q1, q2, q3 with p = 2 + q1 + q2 + q3.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    /* static state to allow out‑of‑order calls */
    static seq_t *primes      = NULL;   /* primes found so far          */
    static size_t   primes_cnt   = 0;    /* number of primes stored      */
    static size_t   primes_cap   = 0;    /* capacity of primes array     */

    static seq_t *results      = NULL;   /* sequence terms found         */
    static size_t  res_cnt      = 0;     /* number of terms stored       */
    static size_t  res_cap      = 0;     /* capacity of results array    */

    /* initialise once */
    if (results == NULL) {
        /* first prime is 2 */
        primes_cap = 16;
        primes = mem_new(seq_t, primes_cap);
        if (!primes) exit(1);
        primes[primes_cnt++] = (seq_t)2;

        /* allocate space for sequence terms */
        res_cap = 32;
        results = mem_new(seq_t, res_cap);
        if (!results) exit(1);
    }

    /* generate terms until the requested index is obtained */
    while (res_cnt < (size_t)n) {
        /* get next prime after the current last one */
        seq_t candidate = primes[primes_cnt - 1] + (seq_t)1;
        while (!is_prime_big(candidate)) candidate++;

        /* append the new prime, expanding array if needed */
        if (primes_cnt == primes_cap) {
            primes_cap <<= 1;
            primes = mem_resize(seq_t, primes, primes_cap);
            if (!primes) exit(1);
        }
        primes[primes_cnt++] = candidate;

        /* test the triple formed by the last three primes */
        if (primes_cnt >= 3) {
            seq_t sum = primes[primes_cnt - 3]
                      + primes[primes_cnt - 2]
                      + primes[primes_cnt - 1]
                      + (seq_t)2;
            if (is_prime_big(sum)) {
                if (res_cnt == res_cap) {
                    res_cap <<= 1;
                    results = mem_resize(seq_t, results, res_cap);
                    if (!results) exit(1);
                }
                results[res_cnt++] = sum;
            }
        }
    }

    return results[n - 1];
}
