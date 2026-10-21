/*
 * A058032  Largest m such that 2^n / primorial(m) >= 1.
 * https://oeis.org/A058032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Max{m | 2^n / primorial(m) >= 1}
 * Range:     u8 n <= 2289, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                       = Max{m | log2(primorial(m)) <= n} */

#include <math.h>
#include <stddef.h>
#include <stdint.h>

/* ---- type aliases supplied by the build ------------------------------- */
 /* (No definitions are provided here; the build will supply seq_t and idx_t). */

/* ---- internal data --------------------------------------------------- */
#define MAX_PRIMES 5000          /* plenty for n up to ~1e4                  */

static unsigned primes[MAX_PRIMES];       /* primes[0] = 2, primes[1] = 3, ...        */
static long double cumLog2[MAX_PRIMES+1]; /* cumLog2[0] = 0                         */
static idx_t primes_cnt   = 0;           /* number of primes stored                 */
static idx_t next_candidate = 3;         /* next odd to test as a prime candidate   */

static const long double LOG2_2 = logl(2.0L);

/* ---- helper: generate next prime ------------------------------------- */
static void add_next_prime(void)
{
    /* find the next prime after next_candidate-1 and store it */
    for ( ; ; ) {
        unsigned p = (unsigned)next_candidate;
        unsigned limit = (unsigned)floorl(sqrtl((long double)p));
        unsigned i, is_prime = 1;
        for (i = 0; i < primes_cnt && primes[i] <= limit; ++i)
            if (p % primes[i] == 0) { is_prime = 0; break; }

        if (is_prime) {
            if (primes_cnt < MAX_PRIMES) {
                primes[primes_cnt] = p;
                cumLog2[primes_cnt+1] = cumLog2[primes_cnt] +
                                        logl((long double)p) / LOG2_2;
                ++primes_cnt;
            }
            next_candidate += 2;
            break;
        }
        next_candidate += 2;   /* only test odd numbers */
    }
}

/* ---- ensure we have enough primes so that cumLog2[last] > n ----------- */
static void ensure_up_to(idx_t n)
{
    /* initialise on first use */
    if (primes_cnt == 0) {
        primes[0] = 2;
        primes_cnt = 1;
        cumLog2[0] = 0.0L;
        cumLog2[1] = logl((long double)2) / LOG2_2;   /* 1.0 */
        next_candidate = 3;
    }

    /* grow until last accumulated log2 exceeds n (with tiny safety margin) */
    const long double eps = 1e-12L;
    while (cumLog2[primes_cnt] <= (long double)n + eps)
        add_next_prime();
}

/* ---- the sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    /* make sure we have enough primes for this n  */
    ensure_up_to(n);

    /* Find the first index m such that cumLog2[m] > n; answer is m-1. */
    idx_t m;
    for (m = 0; m <= primes_cnt && cumLog2[m] <= (long double)n; ++m)
        ;   /* empty body */

    /* m is now first index > n, so largest acceptable m is m-1 */
    return (seq_t)(m - 1);
}
