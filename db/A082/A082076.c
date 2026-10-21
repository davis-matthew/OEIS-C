/*
 * A082076  First differences of primes of the form 4*k+3 (A002145), divided
 *          by 4.
 * https://oeis.org/A082076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p_{n+1} - p_n) / 4 where p_k are primes ≡ 3 (mod 4)
 * Range:     u8 n <= 28, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq_t_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds the list of primes of the form 4k+3 lazily and
   memoises differences.  It uses simple trial division against the
   previously found primes; this is sufficient for n up to several
   million (≈10⁶ gives primes < 2·10⁷).  The implementation works
   for all provided seq_t types. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_seq_t_array.h"

/* static state ------------------------------------------------------- */
static uint64_t *primes   = NULL;    /* all primes found (uint64_t)       */
static size_t   primes_cap   = 0;
static size_t   primes_cnt   = 0;

static seq_t *form_primes = NULL;    /* primes ≡ 3 (mod 4)    */
static size_t form_cap     = 0;
static size_t form_cnt     = 0;

static uint64_t next_candidate = 3;   /* next odd number to test          */

/* ------------------------------------------------------------------- */
/* helper: grow array of uint64_t                                      */
static void grow_uint64_array(uint64_t **arr, size_t *cnt, size_t *cap) {
    if (*cnt >= *cap) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        uint64_t *newp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        if (!newp) abort(); /* out of memory */
        *arr = newp;
        *cap = new_cap;
    }
}

/* ------------------------------------------------------------------- */
/* simple primality test using pre‑computed primes                      */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------- */
/* generate primes until we have at least (need+1) primes ≡ 3 (mod 4)   */
static void ensure_form_primes(size_t need) {
    /* need is the largest index we will access in form_primes */
    while (form_cnt <= need) {
        uint64_t cand = next_candidate;

        bool prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p > cand / p) break; /* p*p > cand */
            if (cand % p == 0) { prime = false; break; }
        }
        if (prime) {
            /* add to all primes list */
            grow_uint64_array(&primes, &primes_cnt, &primes_cap);
            primes[primes_cnt++] = cand;

            /* if ≡3 mod 4, store in form_primes */
            if ((cand & 3ULL) == 3ULL) {
                grow_seq_t_array(&form_primes, &form_cnt, &form_cap);
                form_primes[form_cnt++] = (seq_t)cand;
            }
        }
        next_candidate += 2;
    }
}

/* ------------------------------------------------------------------- */
/* constructor: initialise prime list with 2                       */
static void init_primes_once(void) {
    static bool init_done = false;
    if (init_done) return;
    init_done = true;

    /* seed primes with 2 */
    grow_uint64_array(&primes, &primes_cnt, &primes_cap);
    primes[primes_cnt++] = 2;

    /* start candidate from 3 */
    next_candidate = 3;
}

/* ------------------------------------------------------------------- */
/* public function -------------------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;      /* offset is 1 */

    init_primes_once();              /* ensure 2 is present */

    /* we need form_primes[n] (since a(1) uses form_primes[1] and [0]) */
    ensure_form_primes((size_t)n);

    seq_t prev = form_primes[n - 1];
    seq_t next = form_primes[n];

    return (next - prev) / 4;
}
