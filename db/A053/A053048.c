/*
 * A053048  a(n) is the number of terminal iterations applied to powers of 2
 *          arising in the iterations of the Euler phi function with initial
 *          value n!.
 * https://oeis.org/A053048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = log_2(A053047(n))
 * Range:     u8 n <= 82, u16 n <= 8707, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the exponent of the first power of two reached by repeatedly
 *  applying Euler's totient function to n!.  The implementation keeps only
 *  prime‑factor exponent vectors; the only arithmetic needed is integer
 *  addition, subtraction and multiplication.  It works for any size of
 *  seq_t that can hold the exponent of 2 (which is < n+1 for n ≤ 10000). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Constants -------------------------------------------------------- */

#define PRIME_LIMIT 20000      /* enough for all test values (n ≤ 10000) */

/* ------------------------------------------------------------------ */
/*  Static tables ---------------------------------------------------- */

static uint32_t primes[PRIME_LIMIT + 1];
static uint32_t spf[PRIME_LIMIT + 1];
static int       prime_index[PRIME_LIMIT + 1];
static size_t    prime_cnt = 0;
static bool      primes_inited = false;

/* ------------------------------------------------------------------ */
/*  Prime sieve (once, lazily) -------------------------------------- */

static void init_primes(void)
{
    if (primes_inited)
        return;

    for (size_t i = 0; i <= PRIME_LIMIT; ++i) {
        spf[i] = 0;
        prime_index[i] = -1;
    }

    prime_cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (spf[i] == 0) {             /* i is prime */
            spf[i] = (uint32_t)i;
            prime_index[(int)i] = (int)prime_cnt;
            primes[prime_cnt++] = (uint32_t)i;

            if (i * i <= PRIME_LIMIT) {
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
            }
        }
    }
    primes_inited = true;
}

/* ------------------------------------------------------------------ */
/*  Main function ---------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Initialise primes once */
    init_primes();

    /* Arrays of exponents for the current factorisation and the next
     * iteration.  They are sized to PRIME_LIMIT+1 but only the first
     * prime_cnt entries are used. */
    static idx_t exp[PRIME_LIMIT + 1];
    static idx_t next[PRIME_LIMIT + 1];

    /* Compute Legendre exponents of n! */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if (p > (uint32_t)n) {
            exp[i] = 0;
            continue;
        }
        idx_t e = 0;
        idx_t t = n / p;
        while (t > 0) { e += t; t /= p; }
        exp[i] = e;
    }

    /* Iterate phi until only a power of two remains. */
    while (1) {
        /* If all odd prime exponents are zero, current number is 2^k */
        bool odd_present = false;
        for (size_t i = 1; i < prime_cnt; ++i) { /* skip prime 2 */
            if (exp[i] != 0) {
                odd_present = true;
                break;
            }
        }
        if (!odd_present)
            return (seq_t)exp[0];   /* exponent of 2 in the power of two */

        /* Prepare next exponent vector. */
        for (size_t i = 0; i < prime_cnt; ++i)
            next[i] = 0;

        /* For each prime factor, apply φ(p^e) = p^(e-1)*(p-1). */
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t e = exp[i];
            if (e == 0)
                continue;

            uint32_t p = primes[i];
            if (p == 2) {            /* φ(2^e) = 2^(e-1) */
                next[i] += e - 1;
            } else {
                /* Keep p^(e-1) */
                next[i] += e - 1;

                /* Multiply by (p-1) once – add its prime exponents. */
                uint32_t val = p - 1;
                while (val > 1) {
                    uint32_t q = spf[val];
                    int cnt = 0;
                    while (val % q == 0) {
                        val /= q;
                        ++cnt;
                    }
                    int j = prime_index[(int)q];
                    /* q is guaranteed prime ≤ PRIME_LIMIT, so j >= 0 */
                    next[j] += (idx_t)cnt;
                }
            }
        }

        /* Advance to the next iteration. */
        for (size_t i = 0; i < prime_cnt; ++i)
            exp[i] = next[i];
    }
}
