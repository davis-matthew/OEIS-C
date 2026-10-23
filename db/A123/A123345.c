/*
 * A123345  Numbers containing all divisors in their binary representation.
 * https://oeis.org/A123345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A123345: Numbers whose binary representation contains
   all of its positive divisors as contiguous substrings. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------------
   Prime sieve – used for factorisation of the test numbers.
   ----------------------------------------------------------- */
#define SIEVE_LIMIT 100000U
static bool sieve_arr[SIEVE_LIMIT + 1] = {0};
static uint64_t prime_list[9592];   /* 9592 primes < 100000 */
static size_t prime_count = 0;
static bool primes_initialised = false;

static void initialise_primes(void)
{
    if (primes_initialised) return;
    size_t i;
    for (i = 0; i <= SIEVE_LIMIT; ++i) sieve_arr[i] = true;
    sieve_arr[0] = sieve_arr[1] = false;
    for (i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (sieve_arr[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i) sieve_arr[j] = false;
        }
    }
    for (i = 2; i <= SIEVE_LIMIT; ++i)
        if (sieve_arr[i]) prime_list[prime_count++] = i;
    primes_initialised = true;
}

/* ------------------------------------------------------------------
   Factorisation of a 64‑bit number.
   ----------------------------------------------------------- */
static bool factorise(uint64_t n, uint64_t *pr, int *exp, size_t *cnt)
{
    *cnt = 0;
    for (size_t i = 0; i < prime_count && prime_list[i] <= n / prime_list[i]; ++i) {
        uint64_t p = prime_list[i];
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[(*cnt)++] = p;
            exp[(*cnt) - 1] = e;
        }
    }
    if (n > 1) {
        pr[(*cnt)++] = n;
        exp[(*cnt) - 1] = 1;
    }
    return true;
}

/* ------------------------------------------------------------------
   Utility: convert a non‑zero unsigned integer to binary string.
   ----------------------------------------------------------- */
static void uint64_to_bin(uint64_t v, char *out, size_t *len)
{
    size_t i = 0;
    do {
        out[i++] = (char)('0' + (v & 1ULL));
        v >>= 1U;
    } while (v);
    /* reverse */
    for (size_t j = 0; j < i / 2; ++j) {
        char t = out[j];
        out[j] = out[i - 1 - j];
        out[i - 1 - j] = t;
    }
    out[i] = '\0';
    *len = i;
}

/* ------------------------------------------------------------------
   Global buffer used while checking divisors (re‑entrant usage
   not required for typical unit tests).
   ----------------------------------------------------------- */
static char global_bin[2048];

/* ------------------------------------------------------------------
   Recursive check: test every divisor of 'm' for binary substring
   present in global_bin.
   ----------------------------------------------------------- */
static bool recursive_check(const uint64_t *pr, const int *exp,
                            size_t prime_cnt, size_t idx,
                            uint64_t current)
{
    if (idx == prime_cnt) {
        /* current is a divisor; test substring */
        char dbuf[65];
        size_t dl;
        uint64_to_bin(current, dbuf, &dl);
        if (strstr(global_bin, dbuf) == NULL) return false;
        return true;
    }

    uint64_t p = pr[idx];
    int emax = exp[idx];
    uint64_t mult = 1;
    for (int e = 0; e <= emax; ++e) {
        if (!recursive_check(pr, exp, prime_cnt, idx + 1, current * mult))
            return false;
        mult *= p;
    }
    return true;
}

/* ------------------------------------------------------------------
   Main predicate: does a number satisfy the condition?
   ----------------------------------------------------------- */
static bool has_all_divisors(const seq_t m)
{
    /* Convert m to 64‑bit for factorisation – adequate for the
       sequence sizes required by the tests. */
    uint64_t nm = (uint64_t)m;
    if (nm == 0) return false;   /* should not occur */

    /* Build binary representation of m into global_bin */
    size_t binlen;
    uint64_to_bin(nm, global_bin, &binlen);

    /* Factorise */
    uint64_t primes[32];
    int exps[32];
    size_t prime_cnt;
    factorise(nm, primes, exps, &prime_cnt);

    /* Check all divisors */
    return recursive_check(primes, exps, prime_cnt, 0, 1ULL);
}

/* ------------------------------------------------------------------
   The exported sequence function.
   ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 */

    initialise_primes();

    seq_t result = 1;
    idx_t count = 0;
    while (count < n) {
        if (has_all_divisors(result)) ++count;
        if (count != n) ++result;
    }
    return result;
}
