/*
 * A138757  a(n) = A007918(A138750(n)), that is, least prime > n/2 if n=2 (mod
 *          3), > 2n otherwise.
 * https://oeis.org/A138757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nextprime( A138750(n) ), where * * A138750(n) = ceil(n/2)
 *            if n ≡ 2 (mod 3) * = 2 * n otherwise * * "nextprime" means the
 *            least prime ≥ the given value. The function * is implemented
 *            using a simple deterministic primality test, * which is fast
 *            enough for n up to at least 10 000. The impleme...
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* small deterministic prime test for 64‑bit integers */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if ((x & 1ULL) == 0 || (x % 3ULL) == 0) return false;

    /* trial division by 6k ± 1 */
    for (uint64_t i = 5; i * i <= x; i += 6) {
        if (x % i == 0 || x % (i + 2) == 0) return false;
    }
    return true;
}

/* Compute a(n) for OEIS sequence A138757. */
seq_t a(idx_t n)
{
    /* Compute the intermediate value A138750(n). */
    idx_t step;
    if ((n % 3) == 2) {
        /* ceil(n/2) for unsigned integers: (n+1)/2 */
        step = (n + 1) / 2;
    } else {
        step = n * 2;
    }

    /* Find the least prime ≥ step. */
    uint64_t candidate = step;
    while (!is_prime_uint64(candidate)) {
        ++candidate;
    }

    /* Cast to seq_t; overflow is allowed according to constraints. */
    return (seq_t)candidate;
}
