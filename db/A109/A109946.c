/*
 * A109946  Smallest prime starting a complete two iterations Cunningham chain
 *          of the first and second kind.
 * https://oeis.org/A109946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A109946 – Smallest prime starting a complete two‑iteration
    Cunningham chain of the first or second kind (length 3).
    The function builds the sequence on demand, caching results between
    calls. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/push_seq.h"

/* --- cached sequence data ------------------------------------------ */
static seq_t *seq_vals  = NULL;   /* array of sequence values           */
static idx_t  seq_cap  = 0;       /* current capacity of seq_vals      */
static idx_t  seq_size = 0;       /* number of elements stored         */
static seq_t  next_candidate = 2; /* next integer to test as a prime   */

/* --- helper functions to test chains --------------------------------- */
static bool chain_plus(seq_t p) {
    /* chain with 2p+1 :  p -> q -> r */
    seq_t q = 2 * p + 1;
    if (!is_prime((uint64_t)q)) return false;

    seq_t r = 2 * q + 1;
    if (!is_prime((uint64_t)r)) return false;

    /* no predecessor via +1 */
    if ((p & 1) != 0) {                     /* p odd */
        seq_t prev = (p - 1) >> 1;          /* (p-1)/2 */
        if (is_prime((uint64_t)prev)) return false;
    }
    /* chain must end after r */
    seq_t next = 2 * r + 1;
    if (is_prime((uint64_t)next)) return false;

    return true;
}

static bool chain_minus(seq_t p) {
    /* chain with 2p-1 :  p -> q -> r */
    seq_t q = 2 * p - 1;
    if (!is_prime((uint64_t)q)) return false;

    seq_t r = 2 * q - 1;
    if (!is_prime((uint64_t)r)) return false;

    /* no predecessor via -1 */
    if (p == 2) {
        /* special case – 2 has no integer predecessor via 2x-1 */
    } else {
        seq_t prev = (p + 1) >> 1;          /* (p+1)/2 */
        if (is_prime((uint64_t)prev)) return false;
    }
    /* chain must end after r */
    seq_t next = 2 * r - 1;
    if (is_prime((uint64_t)next)) return false;

    return true;
}

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n) {
    while (seq_size < n) {
        seq_t p = next_candidate;
        /* find next prime ≥ next_candidate */
        while (!is_prime((uint64_t)p)) {
            p++;
        }

        /* does this prime start a valid chain? */
        if (chain_minus(p) || chain_plus(p)) {
            push_seq(&seq_vals, &seq_cap, &seq_size, p);
        }

        /* set up next candidate value */
        next_candidate = (p == 2) ? 3 : p + 2;
    }
    return seq_vals[n - 1];
}
