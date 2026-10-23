/*
 * A045413  Primes congruent to {0, 3, 4} mod 5.
 * https://oeis.org/A045413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A045413: nth prime congruent to 0, 3, or 4 modulo 5. 
 *  We generate primes sequentially and collect those that satisfy the
 *  congruence condition.  The function is memoised, so repeated calls
 *  (in any order) are fast. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *primes        = NULL;   /* all primes generated so far   */
static size_t primes_cap   = 0;
static size_t primes_cnt   = 0;

static seq_t *goodprimes    = NULL;   /* primes that satisfy the congruence */
static size_t good_cap      = 0;
static size_t good_cnt      = 0;

static seq_t next_candidate = 2;      /* next number to test for primality */

/* internal helper: grow an array of seq_t, doubling capacity */
static void grow_array(seq_t **arr, size_t *cap, size_t *cnt, size_t need) {
    if (*cap >= need) return;
    size_t newcap = (*cap == 0) ? 2 : (*cap << 1);
    while (newcap < need) newcap <<= 1;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

/* generate primes until we have at least n good primes */
static void ensure_good(idx_t n) {
    while (good_cnt < n) {
        seq_t num = next_candidate;
        bool prime = true;

        /* trial division by previously found primes */
        for (size_t i = 0; i < primes_cnt; ++i) {
            unsigned long long p = (unsigned long long)primes[i];
            if (p * p > num) break;
            if (num % p == 0) { prime = false; break; }
        }

        if (prime) {
            /* store the newly found prime */
            grow_array(&primes, &primes_cap, &primes_cnt, primes_cnt + 1);
            primes[primes_cnt++] = num;

            /* check congruence condition */
            if (num == 5 || num % 5 == 3 || num % 5 == 4) {
                grow_array(&goodprimes, &good_cap, &good_cnt, good_cnt + 1);
                goodprimes[good_cnt++] = num;
            }
        }

        /* advance to next potential prime */
        if (num == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }
}

/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined but harmless */
    ensure_good(n);
    return goodprimes[n - 1];
}
