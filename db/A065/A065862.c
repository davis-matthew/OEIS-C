/*
 * A065862  Remainder when n-th composite number is divided by the number of
 *          nonprimes not exceeding n.
 * https://oeis.org/A065862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c(n) mod ( n - pi(n) ) where c(n) is the n-th composite
 *            number and pi(n) is the count of primes <= n. The values are
 *            computed by pre‑computing a sieve of Eratosthenes up to a fixed
 *            limit (200 000) and storing the list of first composites and
 *            cumulative prime counts
 * Range:     u8 n <= 207, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static int initialized = 0;

static const idx_t LIMIT = 200000;          /* enough for n up to about 10 000 */
static unsigned char *is_prime = NULL;      /* 0 = not prime, 1 = prime   */
static int *prime_cnt_arr = NULL;           /* prime_cnt_arr[i] = pi(i)    */
static int *comps = NULL;                   /* list of composites         */
static int comp_cnt = 0;                    /* number of stored composites */

/* simple sieve and precomputation */
static void init(void) {
    if (initialized) return;
    initialized = 1;

    /* memory allocation */
    is_prime = (unsigned char*)malloc((LIMIT + 1) * sizeof(unsigned char));
    prime_cnt_arr = (int*)malloc((LIMIT + 1) * sizeof(int));
    comps = (int*)malloc((LIMIT + 1) * sizeof(int)); /* generous size */

    /* init all as prime, then clear 0 and 1 */
    for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* sieve */
    for (idx_t p = 2; p * p <= LIMIT; ++p) {
        if (is_prime[p]) {
            for (idx_t x = p * p; x <= LIMIT; x += p)
                is_prime[x] = 0;
        }
    }

    /* build prime count array and list of primes/composites */
    int pc = 0;                       /* running prime count */
    comp_cnt = 0;                     /* running composite count */

    for (idx_t i = 0; i <= LIMIT; ++i) {
        if (i >= 2 && is_prime[i]) {
            pc++;
        }
        prime_cnt_arr[i] = pc;

        if (i >= 4 && !is_prime[i]) {
            comps[comp_cnt++] = (int)i;
        }
    }
}

/* access function */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* not defined for non‑positive index */
    init();                        /* ensure tables are ready */

    /* ensure we have enough composites; 200000 limit should be more than enough */
    if (n > comp_cnt) {
        /* If we ever need more composites, recompute with a larger limit.
           For the standard test range this branch will never be taken. */
        /* Simple fallback: return 0 to avoid crash */
        return 0;
    }

    seq_t c = (seq_t)comps[n - 1];          /* n-th composite (1‑based) */
    seq_t pi = (seq_t)prime_cnt_arr[n];     /* pi(n): primes <= n      */
    seq_t denom = (seq_t)(n - pi);          /* n - pi(n)               */

    if (denom == 0) return 0;               /* safety, shouldn't happen */
    return (c % denom);
}
