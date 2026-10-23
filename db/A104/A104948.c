/*
 * A104948  Primes from merging of 5 successive digits in decimal expansion of
 *          the Champernowne Constant.
 * https://oeis.org/A104948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A104948
 * Formula: a(n) is the n-th prime that appears as a 5‑digit sliding window
 * over the decimal expansion of Champernowne's constant (digits after the
 * decimal point).  For a window to contribute, its first digit must be non‑zero
 * so that it represents a proper 5‑digit number (10000 … 99999). */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* 5‑digit prime sieve  (0 … 99999) */
static unsigned char *prime_sieve = NULL;
static const size_t prime_sieve_limit = 100000; /* indices 0 … 99999 */

static void init_prime_sieve(void)
{
    if (prime_sieve != NULL)
        return; /* already initialized */

    prime_sieve = (unsigned char *)malloc(prime_sieve_limit);
    memset(prime_sieve, 1, prime_sieve_limit);
    prime_sieve[0] = prime_sieve[1] = 0;

    for (size_t p = 2; p * p < prime_sieve_limit; ++p)
    {
        if (prime_sieve[p])
        {
            for (size_t m = p * p; m < prime_sieve_limit; m += p)
                prime_sieve[m] = 0;
        }
    }
}

/* Return the n‑th prime that occurs as a 5‑digit sliding window in Champernowne's constant.
 *   n ≥ 1; behaviour for n == 0 is undefined (returns 0). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined but harmless */

    init_prime_sieve();

    seq_t window = 0;     /* current 5‑digit window (value 0 … 99999) */
    idx_t  digits_read = 0;   /* how many digits have been seen so far */
    idx_t  primes_found = 0;  /* counter of qualifying primes */

    for (uint64_t number = 1; ; ++number)
    {
        /* Extract decimal digits of 'number' into tmp array (most significant first). */
        int buf[20];
        int len = 0;
        uint64_t tmp = number;
        if (tmp == 0) {
            buf[len++] = 0;
        } else {
            while (tmp > 0) {
                buf[len++] = (int)(tmp % 10);
                tmp /= 10;
            }
        }

        /* Process digits from most significant to least significant. */
        for (int i = len - 1; i >= 0; --i)
        {
            int d = buf[i];

            if (digits_read < 5) {
                /* Build the initial 5‑digit window. */
                ++digits_read;
                window = window * 10 + d;
                if (digits_read == 5 && window >= 10000 &&
                    prime_sieve[(size_t)window]) {
                    ++primes_found;
                    if (primes_found == n)
                        return (seq_t)window;
                }
            } else {
                /* Slide one digit: drop the oldest, add new. */
                window = (window % 10000) * 10 + d;
                if (window >= 10000 &&
                    prime_sieve[(size_t)window]) {
                    ++primes_found;
                    if (primes_found == n)
                        return (seq_t)window;
                }
            }
        }
    }
}
