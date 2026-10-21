/*
 * A226155  Smallest of four consecutive primes whose average is a triangular
 *          number.
 * https://oeis.org/A226155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A226155: the smallest of four consecutive primes whose average is a triangular number.
   A triangular average T means (p+q+r+s)/4 = T.  Triangular numbers T satisfy that 8*T + 1 is a perfect square.
   This implementation scans consecutive primes and records those satisfying the condition. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/floor_sqrt.h"

/* Static cache of primes */
static uint64_t *primes      = NULL;   /* array of primes up to current_limit */
static size_t   primes_cnt   = 0;      /* number of primes in the array */
static idx_t    current_limit = 0;     /* current sieve limit */

/* Static cache of sequence terms */
static seq_t   *terms        = NULL;   /* sequence values found so far   */
static size_t   terms_len    = 0;      /* number of terms stored        */
static size_t   terms_cap    = 0;      /* capacity of terms array       */

/* Index of next quadruple to test in the prime array */
static size_t   next_to_process = 0;

/* Ensure that we have produced at least `need` terms of the sequence. */
static void ensure_terms(idx_t need) {
    if (terms_len >= (size_t)need)
        return;

    /* Initialise the prime list on first use. */
    if (primes == NULL) {
        current_limit = 1000;  /* start with a small sieve */
        build_primes_upto(current_limit, &primes, &primes_cnt);
        next_to_process = 0;
    }

    while (terms_len < (size_t)need) {
        /* Scan available primes for new qualifying quadruples. */
        while (next_to_process + 3 < primes_cnt && terms_len < (size_t)need) {
            uint64_t sum = primes[next_to_process]
                         + primes[next_to_process + 1]
                         + primes[next_to_process + 2]
                         + primes[next_to_process + 3];

            /* Average must be integer: sum divisible by 4. */
            if ((sum & 0x3) == 0) {
                seq_t avg = (seq_t)(sum >> 2);          /* (p+q+r+s)/4 */
                seq_t d   = avg * (seq_t)8 + (seq_t)1;  /* discriminant 8*avg+1 */
                seq_t r   = floor_sqrt(d);              /* integer sqrt   */

                /* r must be integral square root and odd. */
                if (r * r == d && (r & (seq_t)1)) {
                    seq_t smallest = (seq_t)primes[next_to_process];
                    /* Append to terms cache. */
                    if (terms_len == terms_cap) {
                        size_t new_cap = terms_cap ? terms_cap * 2 : 64;
                        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                        if (!tmp) abort();                 /* allocation failure */
                        terms = tmp;
                        terms_cap = new_cap;
                    }
                    terms[terms_len++] = smallest;
                }
            }
            ++next_to_process;
        }

        /* If we still need more terms, enlarge the sieve and rebuild the prime list. */
        if (terms_len < (size_t)need) {
            idx_t new_limit = current_limit * 2;
            free(primes);
            build_primes_upto(new_limit, &primes, &primes_cnt);
            current_limit = new_limit;
            /* next_to_process remains unchanged – it points to the first
               quadruple that could not be completed with the old prime list. */
        }
    }
}

/* Public sequence function required by the build. */
seq_t a(idx_t n) {
    if (n == 0)          /* offset is 1; zero is undefined, return 0 */
        return 0;
    ensure_terms(n);
    return terms[(size_t)n - 1];
}
