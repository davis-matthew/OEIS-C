/*
 * A178145  6n-1,6n+1, 6n+5, 6n+7 are all primes. That is they are adjacent
 *          pairs of twin primes.
 * https://oeis.org/A178145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 84, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find all n such that 6n‑1 and 6n+1 are prime and output the lower
 * n of every adjacent pair of such n.  This implements OEIS sequence
 * A178145.  The code sieves primes up to 6·MAX_GEN+1 and then scans
 * n=1…MAX_GEN; whenever a consecutive pair of n's satisfies the twin
 *‑prime property, the lower n is stored.  The array of stored values
 * is reused in subsequent calls. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static inline bool bit_is_set(const uint8_t *bits, idx_t i)
{
    return (bits[i >> 3] >> (i & 7)) & 1;
}
static inline void bit_clear(uint8_t *bits, idx_t i)
{
    bits[i >> 3] &= ~(1 << (i & 7));
}
static inline void bit_set(uint8_t *bits, idx_t i)
{
    bits[i >> 3] |= 1 << (i & 7);
}

seq_t a(idx_t n)
{
    /* --- one‑time initialisation ------------------------------------------------ */
    static bool ready = false;
    static seq_t *seq   = NULL;          /* array of stored terms          */
    static size_t seq_len = 0;           /* number of terms stored          */
    static const size_t MAX_TERMS = 20000;/* need only first 10000, but keep some spare */
    static const idx_t MAX_GEN   = 6000000;/* generous generation limit          */
    static uint8_t *sieve_bits   = NULL;/* bit sieve for primes             */

    if (!ready) {
        /* 1. build a bit‑sieve up to 6*MAX_GEN+1  */
        const idx_t sieve_limit = 6 * MAX_GEN + 1;
        const size_t bits_len = (sieve_limit >> 3) + 1;
        sieve_bits = (uint8_t *)malloc(bits_len);
        if (!sieve_bits) abort();
        for (size_t i = 0; i < bits_len; ++i) sieve_bits[i] = 0xFF; /* all 1 */

        /* clear 0 and 1 */
        bit_clear(sieve_bits, 0);
        bit_clear(sieve_bits, 1);

        for (idx_t p = 2; p * p <= sieve_limit; ++p) {
            if (!bit_is_set(sieve_bits, p)) continue;
            for (idx_t m = p * p; m <= sieve_limit; m += p)
                bit_clear(sieve_bits, m);
        }

        /* 2. collect the sequence terms (lower n of adjacent pairs) */
        seq = (seq_t *)malloc(MAX_TERMS * sizeof(seq_t));
        if (!seq) abort();

        bool prev_good = false;
        idx_t prev_n   = 0;
        for (idx_t cur = 1; cur <= MAX_GEN && seq_len < MAX_TERMS; ++cur) {
            bool good = bit_is_set(sieve_bits, 6 * cur - 1) &&
                        bit_is_set(sieve_bits, 6 * cur + 1);
            if (good && prev_good)
                seq[seq_len++] = (seq_t)prev_n;
            if (good) { prev_good = true; prev_n = cur; }
            else { prev_good = false; }
        }

        ready = true;
    }

    /* --- answer the query -------------------------------------------------------- */
    if (n == 0 || n > seq_len) return (seq_t)0; /* out of range, defensive */
    return seq[n - 1];
}
