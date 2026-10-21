/*
 * A074398  Number of primes between n and phi(n), with neither n nor phi(n)
 *          included in the count in case they are primes.
 * https://oeis.org/A074398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of primes p with min(n,φ(n)) < p < max(n,φ(n)),
 *            excluding n and φ(n) themselves
 * Range:     u8 n <= 1603, u16 n <= 8652, u32 n <= 8652, u64 n <= 8652,
 *            u128 n <= 8652, big any n
 * Helpers:   build_primes, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/build_primes.h"

/* binary search: first index with arr[idx] >= val */
static size_t lower_bound(const idx_t *arr, size_t n, idx_t val)
{
    size_t l = 0, r = n;
    while (l < r) {
        size_t m = l + (r - l) / 2;
        if (arr[m] < val)
            l = m + 1;
        else
            r = m;
    }
    return l;
}

seq_t a(idx_t n)
{
    seq_t phi_seq = phi(n);
    idx_t phi_n = (idx_t)phi_seq;           /* φ(n) ≤ n for n ≥ 1 */

    idx_t low  = (n < phi_n) ? n : phi_n;
    idx_t high = (n > phi_n) ? n : phi_n;

    if (low + 1 > high - 1)                 /* no numbers strictly between */
        return (seq_t)0;

    idx_t lo = low + 1;                     /* first candidate */
    idx_t hi = high - 1;                    /* last candidate */

    static idx_t *prime_list = NULL;
    static size_t prime_count = 0;
    static idx_t prime_limit = 0;

    if (hi > prime_limit) {
        if (prime_list) {
            free(prime_list);
            prime_list = NULL;
        }
        build_primes(hi + 1, &prime_list, &prime_count); /* primes up to hi inclusive */
        prime_limit = hi + 1;
    }

    size_t start = lower_bound(prime_list, prime_count, lo);
    size_t end   = lower_bound(prime_list, prime_count, hi + 1);
    return (seq_t)(end - start);
}
