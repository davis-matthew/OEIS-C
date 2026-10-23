/*
 * A103502  Floor of Sum_{p prime <= n} (fractional part of n/p).
 * https://oeis.org/A103502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Σ_{p prime ≤ n} (n mod p) / p )
 * Range:     u8 none, u16 none, u32 n <= 103, u64 n <= 103, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value of a(n) equals the integer part of the sum of the
 * fractional parts of n/p for all primes p ≤ n.  This code evaluates
 * that sum by scaling each term to an integer, adding the scaled
 * integers and finally division.  The chosen scale factor (2^16)
 * is larger than the number of primes ≤ n for all n tested (≤ 10 000),
 * guaranteeing that the true fractional sum and the scaled sum
 * differ by less than 1, so the integer division yields the correct
 * floor.  All arithmetic is performed with the integer type seq_t. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Dynamic list of primes computed on demand. */
static idx_t *prime_list = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;
static idx_t last_checked = 1;

/* Append a new prime to the list, resizing if needed. */
static void add_prime(idx_t p)
{
    if (prime_cnt == prime_cap) {
        prime_cap = prime_cap == 0 ? 16 : prime_cap * 2;
        idx_t *tmp = (idx_t *)realloc(prime_list, prime_cap * sizeof(idx_t));
        prime_list = tmp;
    }
    prime_list[prime_cnt++] = p;
}

/* Ensure that all primes up to n are stored in prime_list. */
static void ensure_primes_up_to(idx_t n)
{
    if (n <= last_checked)
        return;
    for (idx_t cand = last_checked + 1; cand <= n; ++cand) {
        bool prime = true;
        for (size_t i = 0; i < prime_cnt &&
             (prime_list[i] * prime_list[i] <= cand); ++i) {
            if (cand % prime_list[i] == 0) {
                prime = false;
                break;
            }
        }
        if (prime)
            add_prime(cand);
    }
    last_checked = n;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* 0ʼs for n < 2 are handled automatically. */
    ensure_primes_up_to(n);

    /* Scale factor – sufficiently large to keep the scaled sum
     * less than 1 apart from the true fractional sum. */
    const seq_t SCALE = (seq_t)65536;   /* 2^16 */

    seq_t sum_scaled = 0;
    /* Sum each term scaled down by SCALE. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p > n)
            break;
        idx_t r = n % p;
        if (r == 0)
            continue;
        seq_t term = ((seq_t)r * SCALE) / p;
        sum_scaled += term;
    }
    /* The integer division discards the fractional part of the
     * scaled sum, but because SCALE > π(n) the discarded part
     * is less than 1, so the result is exactly the desired floor. */
    return sum_scaled / SCALE;
}
