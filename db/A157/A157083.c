/*
 * A157083  Depression-type primes with five digits; from left to right digits
 *          decrease to and increase from the central digit.
 * https://oeis.org/A157083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   a(n) computes the n‑th 5‑digit prime whose decimal digits
 *            strictly decrease to the middle digit and then strictly
 *            increase. That is, for a prime d1 d2 d3 d4 d5 we require d1 > d2
 *            > d3 < d4 < d5 The sequence is finite; it has 309 terms. The
 *            function generates the list on the first call and then re...
 * Range:     u8 none, u16 n <= 112, u32 n <= 309, u64 n <= 309, u128 n <= 309,
 *            big any n
 * Helpers:   pattern_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/pattern_ok.h"

#define MIN 10000
#define MAX 99999
#define LIST_SIZE 309   /* known number of terms */

static seq_t prime_list[LIST_SIZE];
static size_t prime_count = 0;
static bool initialized = false;

/* Build the list of primes once. */
static void init_list(void)
{
    if (initialized)
        return;
    initialized = true;

    /* Sieve of Eratosthenes up to MAX.  The array is a fixed‑size
       static array so it is valid in both C and C++. */
    static unsigned char sieve[MAX + 1];
    for (int i = 2; i <= MAX; ++i)
        sieve[i] = 1;
    for (int i = 2; i * i <= MAX; ++i)
        if (sieve[i])
            for (int j = i * i; j <= MAX; j += i)
                sieve[j] = 0;

    /* Collect primes matching the digit pattern. */
    for (int p = MIN; p <= MAX; ++p) {
        if (sieve[p] && pattern_ok((seq_t)p)) {
            if (prime_count < LIST_SIZE) {
                prime_list[prime_count++] = (seq_t)p;
            }
        }
    }
}

/* Externally visible function as required. */
seq_t a(idx_t n)
{
    init_list();
    if (n < 1 || n > (idx_t)prime_count)
        return (seq_t)0;
    return prime_list[(size_t)(n - 1)];
}
