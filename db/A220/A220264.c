/*
 * A220264  Smallest integer with exactly n semiprime divisors.
 * https://oeis.org/A220264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest integer m such that the number of semiprime
 *            divisors of m equals n. If m = Π p_i^{e_i} then this number is
 *            C(k,2) + #{i : e_i ≥ 2}, where k is the number of distinct
 *            primes of m. Thus a(n) is the minimum over all integers k,t with
 *            C(k,2)+t = n, 0 ≤ t ≤ k, of Π_{i=1}^{t} p_i^2 · Π_...
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 63, u128 n <= 150,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Helper: generate first NMAX primes once.
   Here NMAX = 2000 is more than enough for n ≤ 10000. */
static uint64_t primes[2000];
static size_t prime_cnt = 0;
static void init_primes(void)
{
    if (prime_cnt != 0) return;          // already done

    const uint32_t LIMIT = 20000;        // enough to contain 2000 primes
    bool mark[LIMIT + 1];
    for (uint32_t i = 0; i <= LIMIT; ++i) mark[i] = true;
    mark[0] = mark[1] = false;

    for (uint32_t i = 2; i * i <= LIMIT; ++i) {
        if (mark[i]) {
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                mark[j] = false;
        }
    }

    for (uint32_t i = 2; i <= LIMIT && prime_cnt < 2000; ++i) {
        if (mark[i]) primes[prime_cnt++] = i;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    init_primes();                       /* ensure primes[] filled */

    seq_t best = 0;
    bool found = false;

    for (idx_t k = 1; k <= (idx_t)prime_cnt; ++k) {
        idx_t c = k * (k - 1) / 2;        /* C(k,2) */
        if (c > n) break;                /* no larger k can work */

        idx_t t = n - c;
        if (t > k) continue;             /* t must be ≤ k */

        /* Compute candidate m */
        seq_t m = (seq_t)1;
        for (idx_t i = 0; i < k; ++i) {
            seq_t p = (seq_t)primes[i];
            if (i < t) m = m * p * p;    /* exponent 2 for first t primes */
            else       m = m * p;        /* exponent 1 for the rest */
        }

        if (!found || m < best) {
            best = m;
            found = true;
        }
    }

    /* It should always find a value, but guard just in case */
    return found ? best : (seq_t)0;
}
