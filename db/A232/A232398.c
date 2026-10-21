/*
 * A232398  Number of ways to write n = p + (2^k - k) + (2^m - m) with p prime
 *          and 0 < k <= m.
 * https://oeis.org/A232398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as p + (2^k - k) + (2^m -
 *            m) with p prime and 0 < k <= m. The code brute‑forces all pairs
 *            (k,m) up to k <= m <= 63; this is sufficient for all 64‑bit n,
 *            and for the smaller seq_t types the range is naturally curtailed
 *            by overflow
 * Range:     u8 n <= 403, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/pow2.h"

#define MAX_E 63               /* upper bound for the exponents (2^63 fits in 64 bits) */

static bool initialized = false;
static seq_t val[MAX_E + 1];    /* val[e] = 2^e - e, for e >= 1 */

/* Initialise val[] only once. */
static void init_val(void)
{
    if (initialized) return;
    for (int e = 1; e <= MAX_E; ++e) {
        val[e] = pow2(e) - (seq_t)e;
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    init_val();

    /* For n less than the offset we return 0. */
    if (n < 4) return (seq_t)0;

    idx_t limit = n - 2;            /* maximum allowed value of (2^k-k)+(2^m-m) */
    seq_t count  = 0;

    for (int k = 1; k <= MAX_E; ++k) {
        idx_t vk = (idx_t)val[k];
        if (vk > limit) break;      /* further k will only increase the sum */

        for (int m = k; m <= MAX_E; ++m) {
            idx_t vm = (idx_t)val[m];
            idx_t term = vk + vm;
            if (term > limit) break; /* monotonic in m */

            idx_t p = n - term;      /* candidate prime */
            if (p < 2) continue;     /* prime must be at least 2 */

            if (is_prime(p)) ++count;
        }
    }

    return count;
}
