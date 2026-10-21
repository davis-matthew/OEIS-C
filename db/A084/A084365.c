/*
 * A084365  Squarefree kernel of numbers that are not prime powers.
 * https://oeis.org/A084365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = squarefree kernel of the n‑th integer that is NOT a prime
 *            power (A007947(A024619(n))). The function builds the sequence on
 *            demand, storing the already computed terms in a static array
 * Range:     u8 n <= 185, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------------------------------------------------- */
/* Parameters */
/* ----------------------------------------------------- */
#define MAX_PRIME     2000000      /* enough for n up to at least 100 000 */

/* ----------------------------------------------------- */
/* Static state */
/* ----------------------------------------------------- */
/* Prime sieve */
static uint8_t *is_composite = NULL;   /* flags for composite numbers */
static uint32_t *primes = NULL;        /* list of primes up to MAX_PRIME */
static size_t primes_cnt = 0;

/* Sequence storage */
static seq_t *rad_seq = NULL;          /* precomputed a(k) values */
static size_t rad_len = 0;             /* number of terms stored  */
static size_t rad_cap = 0;             /* capacity of rad_seq          */

/* Last integer inspected while building the sequence */
static uint32_t last_checked = 1;

/* ----------------------------------------------------- */
/* Helper: initialise primes and the sequence storage */
/* ----------------------------------------------------- */
static void init_once(void)
{
    static int done = 0;
    if (done) return;
    done = 1;

    /* ----  Sieve of Eratosthenes up to MAX_PRIME  ---- */
    is_composite = (uint8_t*)malloc(MAX_PRIME + 1);
    for (size_t i = 0; i <= MAX_PRIME; ++i) is_composite[i] = 0;

    /* Rough upper bound for π(MAX_PRIME) ≈ MAX_PRIME / log(MAX_PRIME) */
    size_t est = (MAX_PRIME + 1) / 10;
    primes = (uint32_t*)malloc(est * sizeof(uint32_t));
    primes_cnt = 0;

    for (size_t i = 2; i <= MAX_PRIME; ++i) {
        if (!is_composite[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            if ((uint64_t)i * (uint64_t)i <= (uint64_t)MAX_PRIME)
                for (size_t j = (size_t)i * (size_t)i; j <= (size_t)MAX_PRIME; j += i)
                    is_composite[j] = 1;
        }
    }

    /* ----  Allocate space for the sequence  ---- */
    rad_cap = 128;
    rad_seq = (seq_t*)malloc(rad_cap * sizeof(seq_t));
}

/* ----------------------------------------------------- */
/* Ensure rad_seq has at least n terms */
/* ----------------------------------------------------- */
static void ensure_n(idx_t n)
{
    init_once();

    while (rad_len < (size_t)n) {
        /* Advance to next integer */
        ++last_checked;
        uint32_t v = last_checked;

        /* Determine distinct prime factor count and squarefree product */
        uint32_t x = v;
        seq_t rad = (seq_t)1;
        size_t distinct = 0;

        for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= (uint64_t)x; ++i) {
            uint32_t p = primes[i];
            if (x % p == 0) {
                rad = rad * (seq_t)p;          /* one copy per distinct prime */
                ++distinct;
                while (x % p == 0) x /= p;
            }
        }
        if (x > 1) {
            rad = rad * (seq_t)x;              /* remaining prime factor */
            ++distinct;
        }

        if (distinct == 1) {
            /* v is a prime power – skip */
            continue;
        }

        /* Store the squarefree kernel */
        if (rad_len >= rad_cap) {
            rad_cap <<= 1;
            rad_seq = (seq_t*)realloc(rad_seq, rad_cap * sizeof(seq_t));
        }
        rad_seq[rad_len++] = rad;
    }
}

/* ----------------------------------------------------- */
/* Public sequence function */
/* ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0 */
    ensure_n(n);
    return rad_seq[n - 1];
}
