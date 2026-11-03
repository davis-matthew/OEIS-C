/*
 * A154520  The prime(n)-th digit of the concatenated nonprimes.
 * https://oeis.org/A154520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the prime(n)-th digit of the concatenated nonprimes
 *            (A141468: “0 1 4 6 8 9 10 12 14 …”). The string starts with a
 *            leading 0, so the first prime is 2 and a(1) is the second digit
 *            of that string
 * Range:     u8 n <= 99, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   build_primes, is_prime, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/build_primes.h"
#include "../utils/is_prime.h"
#include "../utils/num_digits0.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* 1. Find the n-th prime using a sieve bound. */
    idx_t limit = _prime_limit_for_idx(n);
    size_t prime_cnt = 0;
    idx_t *primes = NULL;
    build_primes(limit, &primes, &prime_cnt);

    /* Guard against unexpected failures – should never happen for n>=1. */
    if (n == 0 || n > prime_cnt) {
        free(primes);
        return (seq_t)0;
    }

    idx_t prime_val = primes[n - 1];   /* prime(n) */
    free(primes);

    /* 2. Determine the digit at the 1‑based position prime_val
       in the concatenated nonprimes string. */
    /* The string starts with the digit ‘0’, so we pre‑consume that digit. */
    idx_t target_pos = prime_val;      /* 1‑based index in the string */
    idx_t accumulated  = 1;            /* count of digits processed so far */

    for (idx_t m = 1; ; ++m) {
        if (!is_prime(m)) {            /* m is a non‑prime (including 1) */
            uint64_t d = num_digits0(m);   /* number of decimal digits in m */
            idx_t digit_len = (idx_t)d;

            if (accumulated + digit_len >= target_pos) {
                /* Position of the desired digit inside m (0‑based). */
                idx_t k = target_pos - accumulated - 1;

                /* Compute 10^(digit_len-1-k). */
                idx_t exp = digit_len - 1 - k;
                seq_t pow = pow10(exp);

                /* Extract the k‑th digit from the right. */
                seq_t digit = ((seq_t)m / pow) % (seq_t)10;
                return digit;
            }

            accumulated += digit_len;
        }
    }
}
