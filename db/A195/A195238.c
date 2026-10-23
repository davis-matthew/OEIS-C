/*
 * A195238  Numbers with at least 2 and not more than 3 distinct prime factors
 *          not greater than 7 that are multiples of 7 or of 15.
 * https://oeis.org/A195238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numbers that have 2–3 distinct prime factors (from
 *            {2,3,5,7}) and are multiples of 7 or of 15
 * Range:     u8 n <= 33, u16 n <= 380, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates all 2‑,3‑,5‑,7‑smooth numbers in ascending order
   using a classic Hamming‑style algorithm, keeps their prime–factor mask,
   and extracts the required values into a separate sequence array.  The
   arrays are grown dynamically and cached statically so that calls to a()
   in any order are answered correctly. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* primes and corresponding bit masks */
static const seq_t PRIMES[4] = { (seq_t)2, (seq_t)3, (seq_t)5, (seq_t)7 };
static const uint8_t PRIME_MASK[4] = { 1u, 2u, 4u, 8u };

/* static state */
static bool      initialized = false;
static seq_t   *dp      = NULL;      /* all generated smooth numbers   */
static uint8_t *dp_mask = NULL;      /* mask of distinct primes for dp */
static size_t   dp_size = 0;
static size_t   dp_cap  = 0;
static size_t   idx[4]  = { 0, 0, 0, 0 };  /* indices for next multiples */

static seq_t   *seq    = NULL;      /* final sequence a(n) */
static size_t   seq_size = 0;
static size_t   seq_cap  = 0;

/* ensure capacity for increasing the sequence array */
static void
ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ensure capacity for increasing the dp array */
static void
ensure_dp_cap(size_t needed)
{
    if (needed <= dp_cap) return;
    size_t new_cap = dp_cap ? dp_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    dp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
    dp_mask = (uint8_t *)realloc(dp_mask, new_cap * sizeof(uint8_t));
    dp_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialisation on first call */
    if (!initialized) {
        initialized = true;
        /* allocate one slot for dp (number 1) */
        ensure_dp_cap(1);
        dp[0]      = (seq_t)1;
        dp_mask[0] = 0u;
        dp_size    = 1;
        seq_cap    = 0;
        seq_size   = 0;
        /* indices already zero */
    }

    /* generate until we have enough terms */
    while (seq_size < (size_t)n) {
        seq_t cand[4];
        cand[0] = dp[idx[0]] * PRIMES[0];
        cand[1] = dp[idx[1]] * PRIMES[1];
        cand[2] = dp[idx[2]] * PRIMES[2];
        cand[3] = dp[idx[3]] * PRIMES[3];

        /* find minimum among the candidates */
        seq_t next = cand[0];
        unsigned next_idx = 0;
        for (unsigned p = 1; p < 4; ++p) {
            if (cand[p] < next) {
                next = cand[p];
                next_idx = p;
            }
        }

        /* decide whether this is a new dp value */
        bool is_new = (dp_size == 0) || (next != dp[dp_size - 1]);

        if (is_new) {
            /* grow dp array if necessary */
            ensure_dp_cap(dp_size + 1);
            /* compute mask for this new number */
            uint8_t new_mask = dp_mask[idx[next_idx]] | PRIME_MASK[next_idx];
            dp[dp_size]      = next;
            dp_mask[dp_size] = new_mask;

            /* check if this number qualifies for a(n) */
            unsigned distinct = __builtin_popcount((unsigned)new_mask);
            if (distinct >= 2 && distinct <= 3 &&
                ((next % (seq_t)7) == (seq_t)0 ||
                 (((next % (seq_t)3) == (seq_t)0) &&
                  ((next % (seq_t)5) == (seq_t)0))))
            {
                ensure_seq_cap(seq_size + 1);
                seq[seq_size++] = next;
            }
            ++dp_size;
        }

        /* increment all indices that produced 'next' */
        for (unsigned p = 0; p < 4; ++p) {
            if (cand[p] == next) {
                ++idx[p];
            }
        }
    }

    return seq[(size_t)n - 1];
}
