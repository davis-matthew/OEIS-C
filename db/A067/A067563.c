/*
 * A067563  Product of n-th prime number and n-th composite number.
 * https://oeis.org/A067563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) * composite(n) (OFFSET: 1)
 * Range:     u8 n <= 7, u16 n <= 96, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* --------------------------------------------------------------------
 *  Prime / composite generator.
 *  It keeps static arrays of the first MAX_COUNT primes and composites,
 *  growing only as needed.  Each value is stored as idx_t so that
 *  primes larger than the result type can be held safely.  The result
 *  itself is returned in the compilation‑specific seq_t type.
 * -------------------------------------------------------------------- */

#define MAX_COUNT 200000      /* enough primes/composites for n ≤ 10000 */

static idx_t  primes[MAX_COUNT];
static size_t prime_cnt = 0;

static idx_t  composites[MAX_COUNT];
static size_t comp_cnt  = 0;

static idx_t last_checked = 1;   /* 1 is not prime nor composite */

/* --------------------------------------------------------------------
 *  Ensure sufficient primes and composites are known for index n.
 * -------------------------------------------------------------------- */
static void ensure_n(idx_t n)
{
    while (prime_cnt < n || comp_cnt < n) {
        ++last_checked;
        idx_t cand = last_checked;
        bool   is_prime = true;

        /* Trial division using already known primes. */
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = primes[i];
            if ((idx_t)p * (idx_t)p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            if (prime_cnt < MAX_COUNT) {
                primes[prime_cnt++] = cand;
            } else {
                /* would overflow the buffer – not expected for n ≤ 10000 */
                return;
            }
        } else if (cand > 1) {           /* skip 1 as it's neither */
            if (comp_cnt < MAX_COUNT) {
                composites[comp_cnt++] = cand;
            } else {
                return;
            }
        }
    }
}

/* --------------------------------------------------------------------
 *  Sequence term
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined by OEIS offset, but harmless */

    ensure_n(n);

    idx_t p = primes[n - 1];          /* prime(n)  */
    idx_t c = composites[n - 1];      /* composite(n) */

    return (seq_t)(p * c);            /* multiplication in idx_t, truncation to seq_t */
}
