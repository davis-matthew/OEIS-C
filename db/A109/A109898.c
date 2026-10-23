/*
 * A109898  a(n) is the least integer of the form (n-2)(n-4)...(n-2k)/n. 0 if
 *          no such number exists.
 * https://oeis.org/A109898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer of the form (n-2)(n-4)...(n-2k)/n, * or 0
 *            if no such integer exists. * Implementation notes: * * Compute
 *            all products up to n-2k by incremental prime‑exponent *
 *            accumulation. * * Check divisibility by comparing accumulated
 *            exponents with those * of n. * * If divisible, recons...
 * Range:     u8 n <= 14, u16 n <= 17, u32 n <= 24, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   pow_seq_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_seq_int.h"

/* Prime sieve limits ------------------------------------------------------ */
#define PRIME_SIEVE_LIMIT 10000
#define MAX_PRIME_COUNT   1229   /* number of primes ≤ 10000 */

/* Global arrays for primes ----------------------------------------------- */
static int primes[MAX_PRIME_COUNT];
static int prime_count = 0;

/* Lazy initializer for the prime list ----------------------------------- */
static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    bool is_composite[PRIME_SIEVE_LIMIT + 1] = { false };
    for (int i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[prime_count++] = i;
            if ((int64_t)i * i <= PRIME_SIEVE_LIMIT) {
                for (int j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    done = true;
}

/* Factorization: fills exp[prime_count] with the exponent of each prime
 * into the provided array (zeroed before calling). --------------------- */
static void factor_uint64(uint64_t num, int exp[/*prime_count*/])
{
    int i = 0;
    for (; i < prime_count && ((uint64_t)primes[i] * (uint64_t)primes[i]) <= num; ++i) {
        int p = primes[i];
        while (num % p == 0) {
            num /= p;
            exp[i]++;
        }
    }
    if (num > 1) {
        /* num is a prime larger than sqrt(original num) */
        for (i = 0; i < prime_count; ++i) {
            if (primes[i] == (int)num) {
                exp[i]++;
                break;
            }
        }
    }
}

/* Main sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    init_primes();               /* ensure prime list is ready */

    if (n <= 1) return 0;        /* trivial case for n = 1 */

    /* Factor n ---------------------------------------------------------- */
    int e_n[MAX_PRIME_COUNT] = { 0 };
    factor_uint64((uint64_t)n, e_n);

    /* Accumulator for product exponents ------------------------------- */
    int prod_exp[MAX_PRIME_COUNT] = { 0 };

    seq_t best = 0;              /* 0 indicates no valid quotient found */

    /* Iterate over possible k ------------------------------------------- */
    idx_t maxk = (n - 1) / 2;    /* largest k such that n-2k > 0 */
    for (idx_t k = 1; k <= maxk; ++k) {
        uint64_t term = (uint64_t)n - 2 * k;   /* (n-2k) */
        if (term == 0) break;                 /* safety */

        /* Factor current term and add to cumulative exponents -------
           Use a temporary array to hold the factorization of the term */
        int term_exp[MAX_PRIME_COUNT] = { 0 };
        factor_uint64(term, term_exp);
        for (int i = 0; i < prime_count; ++i)
            prod_exp[i] += term_exp[i];

        /* Check divisibility: prod_exp[i] >= e_n[i] for all primes */
        bool ok = true;
        for (int i = 0; i < prime_count; ++i) {
            if (prod_exp[i] < e_n[i]) {
                ok = false;
                break;
            }
        }
        if (!ok) continue;                 /* product not divisible by n yet */

        /* Construct the quotient by subtracting the exponents of n */
        seq_t val = 1;
        for (int i = 0; i < prime_count; ++i) {
            int exp = prod_exp[i] - e_n[i];
            if (exp > 0) {
                /* multiply prime^exp into val */
                for (int j = 0; j < exp; ++j)
                    val *= (seq_t)primes[i];
            }
        }

        /* Keep the smallest positive value seen */
        if (best == 0 || val < best)
            best = val;
    }

    return best;
}
