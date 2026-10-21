/*
 * A077011  Triangle read by rows: T(n,k) = A002110(n)/prime(n+1-k), k = 1..n.
 * https://oeis.org/A077011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 55, u64 n <= 136,
 *            u128 n <= 359, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077011: Triangle read by rows.
   Let m be the unique integer such that m(m+1)/2 >= n, and let
   k = n - m(m-1)/2   (1 <= k <= m).
   Then a(n) is the product of the first m primes divided by
   the prime indexed (m+1-k) among the first m primes.
   In other words, a(n) = ∏_{i=1,i≠m+1-k}^{m} prime(i).  */

/* Requires that seq_t and idx_t are defined elsewhere.         */
#include <stdint.h>
#include <stddef.h>
#include <math.h>   /* for sqrt used only with double casts */

static idx_t primes[200];
static size_t prime_cnt = 0;

/* Make sure we have at least 'need' primes in the array. */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need) return;
    idx_t candidate = 2;
    while (prime_cnt < need) {
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            if (candidate % primes[i] == 0) { is_prime = 0; break; }
        }
        if (is_prime) primes[prime_cnt++] = candidate;
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* n>=1 according to spec */

    /* Determine row m: smallest m with m(m+1)/2 >= n */
    uint64_t nn = (uint64_t)n;
    uint64_t m = (uint64_t)((sqrt((double)(8.0 * nn + 1.0)) - 1.0) / 2.0);
    while (m * (m + 1) / 2 < nn) ++m;
    while (m > 0 && (m - 1) * m / 2 >= nn) --m;

    /* Position k within row */
    uint64_t prev = (m - 1) * m / 2;
    uint64_t k = nn - prev;                 /* 1‑based */
    uint64_t skip = m + 1 - k;              /* prime index to omit */

    /* Ensure enough primes are available */
    ensure_primes((size_t)m);

    /* Compute product of all primes except the omitted one */
    seq_t prod = (seq_t)1;
    for (uint64_t i = 1; i <= m; ++i) {
        if (i == skip) continue;
        prod *= (seq_t)primes[(size_t)(i - 1)];
    }
    return prod;
}
