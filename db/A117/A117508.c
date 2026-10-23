/*
 * A117508  Denominators of partial sums of the Brun series divided by 5.
 * https://oeis.org/A117508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator( r(n) ) / 5, where r(n) = Σ_{k=1..n} (
 *            1/ltp(k) + 1/(ltp(k)+2) ), and ltp(k) is the lesser member of
 *            the k‑th twin‑prime pair
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* ---------- internal state ---------- */
static seq_t N = 0;          /* current numerator of the sum (reduced) */
static seq_t D = 1;          /* current denominator of the sum (reduced) */
static idx_t known_n = 0;    /* how many terms have been processed */

/* twin prime storage */
static uint64_t *twin_vec = NULL;   /* array of lesser parts of twin primes */
static size_t   twin_count = 0;     /* number of stored twin primes */

/* ---------- helper functions ---------- */

/* PRIME sieve and twin prime list up to at least 'n_needed' twin primes */
static void ensure_twin_primes(idx_t n_needed)
{
    if (twin_count >= n_needed)
        return;

    /* Start with a rough upper bound; it will be increased if necessary. */
    idx_t limit = 2000000;          /* ~12556 twin primes, enough for n<=10000 */
    while (1) {
        /* Free old data if any */
        if (twin_vec) {
            free(twin_vec);
            twin_vec = NULL;
        }

        /* Allocate sieve */
        bool *isprime = (bool *)malloc((size_t)(limit + 1));
        if (!isprime) return;       /* allocation failure – let caller handle */

        for (size_t i = 0; i <= (size_t)limit; ++i)
            isprime[i] = true;
        isprime[0] = isprime[1] = false;

        for (idx_t i = 2; (i * i) <= limit; ++i)
            if (isprime[i]) {
                for (idx_t j = i * i; j <= limit; j += i)
                    isprime[j] = false;
            }

        /* Gather twin primes */
        twin_count = 0;
        twin_vec = (uint64_t *)malloc(16 * sizeof(uint64_t));
        size_t capacity = 16;
        if (!twin_vec) {
            free(isprime);
            return;
        }

        for (idx_t i = 3; i + 2 <= limit; ++i) {
            if (isprime[i] && isprime[i + 2]) {
                if (twin_count == capacity) {
                    capacity *= 2;
                    uint64_t *tmp = (uint64_t *)realloc(twin_vec, capacity * sizeof(uint64_t));
                    if (!tmp) {
                        free(twin_vec);
                        free(isprime);
                        return;
                    }
                    twin_vec = tmp;
                }
                twin_vec[twin_count++] = (uint64_t)i;
                if (twin_count >= n_needed)
                    break;
            }
        }

        free(isprime);

        if (twin_count >= n_needed)
            break;

        /* Need more twin primes – increase limit and try again */
        limit *= 2;
    }
}

/* Safe addition of the term 2(p+1)/(p(p+2)) to the running sum N/D */
static void add_term(uint64_t p)
{
    seq_t pseq      = (seq_t)p;
    seq_t denom     = pseq * (pseq + 2);          /* p*(p+2)   */
    seq_t term_num  = (seq_t)2 * (pseq + 1);      /* 2*(p+1)   */

    /* Compute gcd of current denominator and the new denominator */
    seq_t g1   = gcd(D, denom);
    seq_t D_div_g1    = D / g1;
    seq_t denom_div_g1 = denom / g1;

    /* Numerator of the sum after addition */
    seq_t new_num = N * denom_div_g1 + term_num * D_div_g1;
    seq_t new_den = D_div_g1 * denom;              /* lcm(D, denom) */

    /* Reduce the resulting fraction */
    seq_t g2 = gcd(new_num, new_den);
    N = new_num / g2;
    D = new_den / g2;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* undefined for n < 1, but not used */

    /* Extend computation up to requested n */
    if (known_n < n) {
        ensure_twin_primes(n);          /* guarantee enough twin primes */

        for (idx_t i = known_n; i < n; ++i)
            add_term(twin_vec[i]);

        known_n = n;
    }

    /* D is always divisible by 5 */
    return D / ((seq_t)5);
}
