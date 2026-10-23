/*
 * A103841  Numbers not representable as a sum of b^e with b >= 2, e >= 2, e
 *          distinct.
 * https://oeis.org/A103841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A103841: numbers not representable as a sum of distinct b^e
    (b >= 2, e >= 2, exponents distinct).  The implementation uses a
    bit‑set sieve over all exponents up to a fixed limit that is large
    enough to contain the first 10 000 terms.  Each exponent may be used
    at most once; the algorithm performs a knapsack over the exponents. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ---- configuration ---------------------------------------------------- */
#define LIMIT 500000ULL                   /* upper bound for precomputation */
#define WORDS   ((LIMIT + 63ULL) / 64ULL) /* number of 64‑bit words in bitset */
#define MAX_EXP 25                         /* maximum exponent processed */

/* ---- global state ----------------------------------------------------- */
static bool initialized = false;
static uint64_t reachable[WORDS];
static uint64_t tmp[WORDS];
static uint64_t powers[MAX_EXP][MAX_EXP * 500]; /* sufficient storage */
static idx_t power_cnt[MAX_EXP];

/* ---- helpers ----------------------------------------------------------- */
/* OR into dest the source bitset shifted left by 'shift' bits.  'shift'
   is at most LIMIT.  Both src and dest are WORDS words long. */
static void shift_or(const uint64_t *src, uint64_t *dest, idx_t shift)
{
    idx_t word_shift = shift >> 6;          /* number of whole words to shift   */
    int  bit_shift  = shift & 63;           /* remaining bit shift             */

    /* shift only if there is room for the target word */
    for (idx_t i = 0; i + word_shift < WORDS; ++i) {
        uint64_t v = src[i];
        if (!v) continue;

        idx_t dest_idx = i + word_shift;
        if (bit_shift == 0) {
            dest[dest_idx] |= v;
        } else {
            dest[dest_idx]   |= v << bit_shift;
            if (dest_idx + 1 < WORDS)
                dest[dest_idx + 1] |= v >> (64 - bit_shift);
        }
    }
}

/* tests if number m (<= LIMIT) is representable */
static bool is_representable(idx_t m)
{
    return (reachable[m >> 6] >> (m & 63ULL)) & 1ULL;
}

/* ---- public function --------------------------------------------------- */
seq_t a(idx_t n)
{
    /* one‑time initialization ------------------------------------------- */
    if (!initialized) {
        /* clear reachable bitset; only 0 is reachable initially */
        for (idx_t i = 0; i < WORDS; ++i) reachable[i] = 0;
        reachable[0] = 1ULL;

        /* determine maximum exponent needed so that 2^e <= LIMIT */
        idx_t e_max = 2;
        while ((1ULL << e_max) <= LIMIT && e_max < MAX_EXP) ++e_max;

        /* pre‑compute b^e for each exponent e */
        for (idx_t e = 2; e < e_max; ++e) {
            idx_t k = 0;
            for (idx_t base = 2; ; ++base) {
                uint64_t val = 1;
                for (idx_t p = 0; p < e; ++p) val *= base;
                if (val > LIMIT) break;
                powers[e][k++] = val;
            }
            power_cnt[e] = k;
        }

        /* knapsack over exponents: each exponent may be used at most once */
        for (idx_t e = 2; e < e_max; ++e) {
            /* tmp starts as the old reachable set (skip exponent e) */
            for (idx_t i = 0; i < WORDS; ++i) tmp[i] = reachable[i];

            /* try adding each power of this exponent */
            for (idx_t t = 0; t < power_cnt[e]; ++t)
                shift_or(reachable, tmp, powers[e][t]);

            /* update reachable for next exponent */
            for (idx_t i = 0; i < WORDS; ++i) reachable[i] = tmp[i];
        }

        initialized = true;
    }

    /* search for the n‑th non‑representable number */
    idx_t count = 0;
    for (idx_t val = 1; ; ++val) {
        if (!is_representable(val)) {
            ++count;
            if (count == n) return (seq_t)val;
        }
    }
}
