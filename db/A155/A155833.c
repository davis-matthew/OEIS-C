/*
 * A155833  Primes in which smallest digit is final digit.
 * https://oeis.org/A155833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime_big, seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A155833: primes whose smallest digit is the last digit.
   Computed by generating primes in increasing order and collecting
   those satisfying the digit condition. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

static seq_t *qualprimes = NULL;   /* array of qualifying primes */
static size_t qualcnt   = 0;       /* number of qualifying primes stored */
static size_t qualcap   = 0;       /* current capacity of qualprimes */
static seq_t next_candidate = 2;   /* next number to test for primality */
static bool initialized = false;  /* flag for one‑time initialization */

/* check if a prime satisfies the OEIS condition */
static bool satisfies_condition(seq_t p) {
    seq_t tmp = p;
    unsigned min_digit = 9;
    unsigned last_digit = 0;
    bool first = true;

    while (tmp > (seq_t)0) {
        unsigned d = (unsigned)(tmp % (seq_t)10);
        if (d < min_digit) min_digit = d;
        if (first) {
            last_digit = d;
            first = false;
        }
        tmp /= (seq_t)10;
    }
    return last_digit == min_digit;
}

/* enlarge the array of qualifying primes if needed */
static void ensure_capacity(void) {
    if (qualcnt >= qualcap) {
        size_t newcap = qualcap ? qualcap << 1 : 16;
        seq_t *newarr = seq_alloc_array(newcap);
        for (size_t i = 0; i < qualcnt; ++i) {
            newarr[i] = qualprimes[i];
        }
        seq_free_array(qualprimes);
        qualprimes = newarr;
        qualcap    = newcap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */

    if (!initialized) {
        initialized = true;        /* next_candidate already 2 */
    }

    while (qualcnt < (size_t)n) {
        if (is_prime_big(next_candidate)) {
            if (satisfies_condition(next_candidate)) {
                ensure_capacity();
                qualprimes[qualcnt++] = next_candidate;
            }
        }
        /* advance to next candidate: 2 → 3, then odd numbers only */
        if (next_candidate == (seq_t)2) {
            next_candidate = (seq_t)3;
        } else {
            next_candidate += (seq_t)2;
        }
    }

    return qualprimes[(size_t)n - 1];
}
