/*
 * A145354  It is conjectured that for each m >= 1 there exist primes Q=Q(m)
 *          and P=P(m) with (2m)^2 + 1 <= Q <= (2m+1)^2 - 2m <= P <= (2m+1)^2;
 *          then set a(2m-1) = Q, a(2m) = P.
 * https://oeis.org/A145354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2m-1) = smallest prime in [(2m)^2+1 , (2m+1)^2-2m], a(2m) =
 *            smallest prime in [(2m+1)^2-2m , (2m+1)^2]
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Trial‑division primality test for numbers < 2^64.
   Uses 6k ± 1 optimisation. */
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2)                 return false;
    if (n % 2 == 0)            return n == 2;
    if (n % 3 == 0)            return n == 3;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* Find the smallest prime in the inclusive range [lo, hi]. */
static inline seq_t find_smallest_prime(seq_t lo, seq_t hi)
{
    for (seq_t x = lo; x <= hi; ++x) {
        if (is_prime_uint64((uint64_t)x))
            return x;
    }
    return 0;                    /* should never happen for the problem data */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1, but return 0 for safety */

    /* m = ceil(n/2) for odd n, n/2 for even n */
    if (n & 1U) {               /* odd index → Q(m) */
        idx_t m = (n + 1) / 2;
        uint64_t mm = (uint64_t)m;
        uint64_t two_m = 2 * mm;

        /* bounds for Q interval: [(2m)^2+1 , (2m+1)^2-2m] */
        uint64_t lo = two_m * two_m + 1;                   /* (2m)^2 + 1 */
        uint64_t hi = (two_m + 1) * (two_m + 1) - two_m;   /* (2m+1)^2 - 2m */

        return find_smallest_prime((seq_t)lo, (seq_t)hi);
    } else {                    /* even index → P(m) */
        idx_t m = n / 2;
        uint64_t mm = (uint64_t)m;
        uint64_t two_m = 2 * mm;

        /* bounds for P interval: [(2m+1)^2-2m , (2m+1)^2] */
        uint64_t lo = (two_m + 1) * (two_m + 1) - two_m;   /* (2m+1)^2 - 2m */
        uint64_t hi = (two_m + 1) * (two_m + 1);           /* (2m+1)^2 */

        return find_smallest_prime((seq_t)lo, (seq_t)hi);
    }
}
