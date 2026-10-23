/*
 * A025289  Numbers that are the sum of 2 nonzero squares in exactly 6 ways.
 * https://oeis.org/A025289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 91, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A025289 – numbers that are the sum of two non‑zero squares in
   exactly 6 unordered ways.
   The function generates terms by sieving primes up to a fixed limit,
   counting the ordered integer representations via the classical
   r₂(n) formula, then converting to unordered positive representations
   with the adjustment
        t(n) = (r₂(n) – 4·is_square + 4·even_half_square)/8
   where is_square is 1 if n is a perfect square, and even_half_square is
   1 if n is even and n/2 is a square. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* --- Parameters ----------------------------------------- */
#define LIMIT ((idx_t)20000000ULL)   /* maximum integer to sieve */

/* --- Static storage ------------------------------------- */
static uint32_t *spf_table = NULL;   /* smallest prime factor for each integer ≤ LIMIT   */
static size_t   spf_size   = 0;

static seq_t  *seq_cache   = NULL;   /* array of discovered sequence terms              */
static size_t seq_len     = 0;        /* number of terms stored                          */
static size_t seq_cap     = 0;        /* current capacity of seq_cache                   */

static idx_t   next_candidate = 1;   /* next integer to test for the sequence           */

/* --- Utility: initialise SPF table --------------------- */
static void init_spf(void)
{
    if (spf_table != NULL) return;           /* already initialised */

    spf_size = (size_t)(LIMIT + 1);
    spf_table = (uint32_t *)malloc(spf_size * sizeof(uint32_t));
    if (!spf_table) exit(1);
    for (size_t i = 0; i < spf_size; ++i) spf_table[i] = 0;

    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (spf_table[(size_t)i] == 0) {          /* i is prime */
            spf_table[(size_t)i] = (uint32_t)i;
            if (i * i <= LIMIT) {
                for (uint64_t j = i * i; j <= LIMIT; j += i)
                    if (spf_table[(size_t)j] == 0)
                        spf_table[(size_t)j] = (uint32_t)i;
            }
        }
    }

    /* initialise cache with some space */
    seq_cap = 1024;
    seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (!seq_cache) exit(1);
}

/* --- Count unordered representations with non‑zero squares */
static uint32_t count_representation(uint64_t n)
{
    if (n == 0) return 0;

    uint64_t temp   = n;
    uint64_t prod   = 1;                     /* product over primes ≡1 (mod 4) */
    bool     valid  = true;

    while (temp > 1) {
        uint32_t p   = spf_table[(size_t)temp];
        uint64_t e   = 0;
        while (temp % p == 0) { temp /= p; ++e; }

        if ((p & 3) == 3) {                  /* prime ≡3 (mod 4) */
            if (e & 1) { valid = false; break; }
        } else if (p != 2) {                  /* prime ≡1 (mod 4) */
            prod *= (e + 1);
        }
    }

    if (!valid) return 0;                    /* no representations */

    uint64_t r2 = 4 * prod;                  /* total integer representations */

    /* square tests */
    uint64_t s = (uint64_t)sqrt((double)n);
    bool is_square = (s * s == n);

    bool even_half_square = false;
    if ((n & 1) == 0) {
        uint64_t h = n >> 1;
        uint64_t t = (uint64_t)sqrt((double)h);
        even_half_square = (t * t == h);
    }

    uint64_t nb = (r2 - (is_square ? 4 : 0) + (even_half_square ? 4 : 0)) / 8;
    return (uint32_t)nb;                     /* fits in 32 bits for our limits */
}

/* --- Public function ------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for n≤0 */

    init_spf();

    while (seq_len < (size_t)n) {
        uint64_t cand = (uint64_t)next_candidate++;

        if (cand > (uint64_t)LIMIT) {        /* out of pre‑computed range */
            /* In practice this should not happen for n ≤ 10 000. */
            exit(1);
        }

        if (count_representation(cand) == 6) {
            if (seq_len == seq_cap) {        /* grow cache if needed */
                seq_cap *= 2;
                seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
                if (!seq_cache) exit(1);
            }
            seq_cache[seq_len++] = (seq_t)cand;
        }
    }

    return seq_cache[(size_t)n - 1];
}
