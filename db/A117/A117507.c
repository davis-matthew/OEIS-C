/*
 * A117507  Numerators of partial sums of the Brun series divided by 4.
 * https://oeis.org/A117507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of the partial sum of the Brun series *
 *            (sum_{k=1..n} (1/ltp(k)+1/(ltp(k)+2))) divided by 4. * ltp(k) is
 *            the lesser twin prime of the k‑th twin pair (offset 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t *a_vals = NULL;           /* cached a(n) values */
static size_t  a_cap  = 0;
static size_t  a_cnt  = 0;

/* prime sieve cache */
static uint64_t *prime_list = NULL;
static size_t   prime_cap  = 0;
static size_t   prime_cnt  = 0;
static uint64_t last_prime = 0;      /* last prime found */
static uint64_t candidate  = 0;      /* next odd number to test */

/* current Brun partial sum (numerator/denominator) */
static seq_t sum_num = (seq_t)0;
static seq_t sum_den = (seq_t)1;

/* generate the next prime and return it */
static uint64_t next_prime_cached(void)
{
    if (prime_cnt == 0) {
        prime_cap  = 16;
        prime_list = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        prime_list[0] = 2ULL;
        prime_cnt    = 1;
        last_prime   = 2ULL;
        candidate    = 3ULL;
    }

    for (;;) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap * 2;
                prime_list = (uint64_t*)realloc(prime_list,
                                                prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = candidate;
            last_prime = candidate;
            candidate += 2ULL;   /* next odd */
            return last_prime;
        }
        candidate += 2ULL;
    }
}

/* compute a(n) for all n up to target (1‑based) */
static void compute_up_to(idx_t target)
{
    if (target == 0 || target <= a_cnt) return;

    while (a_cnt < (size_t)target) {
        uint64_t prev_prime = last_prime;
        uint64_t new_prime  = next_prime_cached();

        /* check for twin prime pair (prev_prime, new_prime) */
        if (prev_prime != 0 && new_prime == prev_prime + 2ULL) {
            uint64_t p = prev_prime; /* lesser twin prime */

            /* term = 2*(p+1) / (p*(p+2)) */
            seq_t term_num = (seq_t)2 * ((seq_t)p + (seq_t)1);
            seq_t term_den = (seq_t)p * ((seq_t)p + (seq_t)2);

            /* add term to current sum:
             * sum_num/sum_den + term_num/term_den
             */
            seq_t g   = gcd(sum_den, term_den);
            seq_t mul1 = term_den / g;
            seq_t mul2 = sum_den / g;

            seq_t new_num = sum_num * mul1 + term_num * mul2;
            seq_t new_den = mul2 * term_den;     /* lcm(sum_den,term_den) */

            seq_t g2 = gcd(new_num, new_den);
            sum_num = new_num / g2;
            sum_den = new_den / g2;

            /* store a(n) = sum_num / 4 */
            if (a_cnt == a_cap) {
                a_cap = a_cap ? a_cap * 2 : 16;
                a_vals = (seq_t*)realloc(a_vals,
                                         a_cap * sizeof(seq_t));
            }
            a_vals[a_cnt++] = sum_num / (seq_t)4;
        }
    }
}

/* public OEIS function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* offset 1 */

    compute_up_to(n);
    return a_vals[n - 1];
}
