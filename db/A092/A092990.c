/*
 * A092990  Least product of the partitions of n into two parts with maximal
 *          tau value: let n = a+b be a partition of n, then a(n) = a*b such
 *          that tau(a*b) is maximal.
 * https://oeis.org/A092990
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least a*b for a+b=n that has the maximal divisor count
 *            τ(a*b)
 * Range:     u8 n <= 34, u16 n <= 519, u32 n <= 8364, u64 n <= 8364,
 *            u128 n <= 8364, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static data ------------------------------------------------ */
/* Prime list up to a generous limit; suffices for n up to 100 000 */
static uint32_t *prime_list   = NULL;
static size_t   prime_count   = 0;
static bool     primes_ready  = false;

/* ----------------------------------------------------------------
   Build a list of primes up to a fixed upper bound.
   The bound is chosen large enough for the required n (≈10 000).
   ---------------------------------------------------------------- */
static void init_primes(void)
{
    if (primes_ready) return;

    const idx_t MAXP = 100000;          /* covers n up to about 100 000 */
    bool *is_comp = (bool *)malloc((MAXP + 1) * sizeof(bool));
    for (idx_t i = 0; i <= MAXP; ++i) is_comp[i] = false;
    is_comp[0] = is_comp[1] = true;

    for (idx_t p = 2; p * p <= MAXP; ++p) {
        if (!is_comp[p]) {
            for (idx_t q = p * p; q <= MAXP; q += p)
                is_comp[q] = true;
        }
    }

    size_t cnt = 0;
    for (idx_t i = 2; i <= MAXP; ++i)
        if (!is_comp[i]) ++cnt;

    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    prime_count = cnt;
    size_t pos = 0;
    for (idx_t i = 2; i <= MAXP; ++i)
        if (!is_comp[i]) prime_list[pos++] = (uint32_t)i;

    free(is_comp);
    primes_ready = true;
}

/* ----------------------------------------------------------------
   Count the number of divisors τ(x) for x formed from seq_t.
   The factorisation uses the pre‑computed primes.
   ---------------------------------------------------------------- */
static seq_t divisor_count(seq_t x)
{
    seq_t result = 1;
    seq_t temp   = x;

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p   = prime_list[i];
        seq_t p_sq   = (seq_t)p * (seq_t)p;
        if (p_sq > temp) break;               /* all remaining factors are > √temp */

        if (temp % p == 0) {
            uint8_t exp = 0;
            do {
                temp /= p;
                ++exp;
            } while (temp % p == 0);
            result *= (seq_t)(exp + 1);
        }
    }

    if (temp > 1)   /* remaining prime factor */
        result *= 2;

    return result;
}

/* ----------------------------------------------------------------
   The required function:
   a(idx_t n) :  the least product a*b of a partition n = a+b
   having the maximum divisor count τ(a*b).
   ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* undefined for n < 2 */

    init_primes();

    seq_t best_product = (seq_t)-1;   /* initialise to max value of seq_t */
    seq_t best_tau     = 0;

    for (idx_t a_part = 1; a_part < n; ++a_part) {
        idx_t b_part  = n - a_part;
        seq_t prod    = (seq_t)a_part * (seq_t)b_part;
        seq_t tau_val = divisor_count(prod);

        if (tau_val > best_tau ||
            (tau_val == best_tau && prod < best_product)) {
            best_tau     = tau_val;
            best_product = prod;
        }
    }

    return best_product;
}
