/*
 * A101184  a(n) = n + pi(n) + pi(pi(n)) + pi(pi(pi(n))) + pi(pi(pi(pi(n)))) +
 *          ...
 * https://oeis.org/A101184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + pi(n) + pi(pi(n)) + pi(pi(pi(n))) +
 * Range:     u8 n <= 188, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t max_sieved = 0;
static unsigned char *sieve = NULL;
static idx_t *pi_arr = NULL;

/* Extend the prime sieve to at least n.
   Recomputes the entire sieve each time for simplicity;
   n is small (≈10 000). */
static void sieve_extend(idx_t n) {
    if (n <= max_sieved)
        return;
    max_sieved = n;
    sieve = (unsigned char *)realloc(sieve, (size_t)(n + 1));
    pi_arr = (idx_t *)realloc(pi_arr, (size_t)(n + 1) * sizeof(idx_t));
    if (!sieve || !pi_arr)
        return;  /* Allocation failure – caller will not get a correct value */

    for (idx_t i = 0; i <= n; ++i)
        sieve[i] = 1;
    if (n >= 0) sieve[0] = 0;
    if (n >= 1) sieve[1] = 0;
    for (idx_t i = 2; i * i <= n; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                sieve[j] = 0;
        }
    }
    idx_t count = 0;
    for (idx_t i = 0; i <= n; ++i) {
        if (sieve[i])
            ++count;
        pi_arr[i] = count;
    }
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;
    seq_t sum = (seq_t)n;
    idx_t val = n;
    while (1) {
        if (val > max_sieved)
            sieve_extend(val);
        idx_t p = pi_arr[val];
        if (p == 0)
            break;
        sum += (seq_t)p;
        val = p;
    }
    return sum;
}
