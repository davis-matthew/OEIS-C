/*
 * A143655  Triangle read by rows, primes not dividing n; A054521 * (A061397 *
 *          0^(n-k)), 1<=k<=n.
 * https://oeis.org/A143655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = k if k is prime and k does not divide r; otherwise 0
 *            where r is the unique integer with r(r+1)/2 >= n and k = n -
 *            (r-1)r/2
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   is_prime_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_generic.h"

#define MAX_PRIME 2000000  /* upper bound for pre‑computed primes */

static unsigned char prime_flag[MAX_PRIME + 1];
static bool primes_prepared = false;

static void prepare_primes(void) {
    if (primes_prepared) return;
    for (idx_t i = 0; i <= MAX_PRIME; ++i) prime_flag[i] = 1;
    prime_flag[0] = prime_flag[1] = 0;
    for (idx_t i = 2; i * i <= MAX_PRIME; ++i) {
        if (prime_flag[i]) {
            for (idx_t j = i * i; j <= MAX_PRIME; j += i) prime_flag[j] = 0;
        }
    }
    primes_prepared = true;
}

static bool is_prime_idx(idx_t p) {
    if (p <= MAX_PRIME) return prime_flag[p];
    return is_prime_generic(p);
}

/* binary search for the row index r such that r(r+1)/2 >= n */
static idx_t find_row(idx_t n) {
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* safeguard, though a(0) is unused */
    prepare_primes();

    idx_t r = find_row(n);                               /* row number */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - prev;                                  /* column number (1‑based) */

    if (is_prime_idx(k) && (r % k != 0)) return (seq_t)k;
    return (seq_t)0;
}
