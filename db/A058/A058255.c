/*
 * A058255  Distinct values of lcm_{i=1..n} (p(i)-1), where p() are the
 *          primes.
 * https://oeis.org/A058255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 12, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Distinct values of lcm_{i=1..k} (p(i)-1) where p(i) are the primes.
   a(n) is the nth distinct value, with a(1)=1. */

#include <stdlib.h>
#include <stdbool.h>

/* --------- helper: gcd for seq_t ------------------------------------ */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* --------- static storage ------------------------------------------- */
static seq_t *values = NULL;      /* array of distinct LCM values       */
static size_t values_len  = 0;    /* number of stored values           */
static size_t values_cap  = 0;    /* allocated capacity                */

static idx_t *primes = NULL;     /* array of primes encountered       */
static size_t primes_len = 0;     /* number of primes                  */
static size_t primes_cap = 0;     /* allocated capacity                */

static seq_t current_lcm   = 1;   /* LCM of (p-1) up to current prime */
static idx_t next_candidate = 3;  /* next odd to test for primality   */

/* --------- vector helpers ------------------------------------------ */
static void push_value(seq_t v)
{
    if (values_len >= values_cap) {
        size_t new_cap = values_cap ? values_cap * 2 : 32;
        seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        values = tmp;
        values_cap = new_cap;
    }
    values[values_len++] = v;
}

static void push_prime(idx_t p)
{
    if (primes_len >= primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 32;
        idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        if (!tmp) exit(EXIT_FAILURE);
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_len++] = p;
}

/* --------- main sequence routine ------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* out of bounds for the sequence */

    /* initialise on first call */
    if (values_len == 0) {
        push_prime((idx_t)2);              /* first prime,  p-1 = 1 */
        push_value((seq_t)1);              /* a(1) = 1 */
    }

    /* generate terms until we have at least n of them */
    while (values_len < (size_t)n) {
        /* -------- find next prime -------- */
        idx_t p;
        for (;;) {
            bool is_prime = true;
            for (size_t i = 0; i < primes_len; ++i) {
                idx_t q = primes[i];
                /* stop when q > sqrt(p) */
                if ((unsigned long long)q * (unsigned long long)q > next_candidate)
                    break;
                if (next_candidate % q == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                p = next_candidate;
                break;
            }
            next_candidate += 2;          /* skip even numbers */
        }

        /* store the new prime and prepare for next candidate */
        push_prime(p);
        next_candidate += 2;

        /* -------- update lcm with (p-1) -------- */
        seq_t p_minus = (seq_t)(p - 1);
        seq_t g = gcd_seq_t(current_lcm, p_minus);
        current_lcm = (current_lcm / g) * p_minus;

        /* append only if the LCM changed */
        if (values[values_len - 1] != current_lcm)
            push_value(current_lcm);
    }

    return values[n - 1];                  /* zero‑based index in array   */
}
