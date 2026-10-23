/*
 * A123856  Primes p that divide A123855(p-1).
 * https://oeis.org/A123856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 199, u32 n <= 199, u64 n <= 199,
 *            u128 n <= 199, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A123856: 
   The nth term is the nth prime p that divides A123855(p-1). 
   Computation is performed by a sieve of primes up to a fixed limit, 
   and by counting for each prime p the number of primes q among the 
   first p‑1 primes for which p | (q-1).  If that count is a multiple 
   of p, then p is included in the sequence. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* Configuration: adjust LIMIT for the expected range of terms   */
/* The chosen limit guarantees the first 10000 terms of the     */
/* sequence are computed (≈ few hundred thousand primes).       */
/* ------------------------------------------------------------ */
static const idx_t LIMIT = 5000000ULL;   /* safe upper bound for 10k terms */

/* ------------------------------------------------------------ */
/*  Global data structures                                       */
/* ------------------------------------------------------------ */
static uint32_t *spf = NULL;            /* smallest prime factor */
static uint32_t *primes = NULL;         /* list of primes in order */
static size_t    prime_count = 0;        /* number of primes found   */
static uint32_t *cnt = NULL;            /* count of q with p|(q-1)  */
static seq_t   *answer_vals = NULL;     /* primes that satisfy the condition */
static size_t    answer_count = 0;
static bool     initialized = false;

/* ------------------------------------------------------------ */
/* Helper: initialise all data (only once)                      */
/* ------------------------------------------------------------ */
static void init(void)
{
    /* ---- 1. Allocate arrays -------------------------------- */
    spf = (uint32_t*)calloc((size_t)(LIMIT + 1ULL), sizeof(uint32_t));
    if (!spf) exit(1);

    /* Sieve to compute primes and smallest prime factors      */
    size_t cap = LIMIT / 10 + 16;
    primes = (uint32_t*)calloc(cap, sizeof(uint32_t));
    if (!primes) exit(1);

    /* -------------------------------------------------------- */
    /* 2. Eratosthenes sieve with spf                           */
    /* -------------------------------------------------------- */
    for (uint32_t i = 2; i <= LIMIT; ++i)
    {
        if (spf[i] == 0U)                /* i is prime */
        {
            spf[i] = i;
            if (prime_count == cap)      /* grow the primes buffer */
            {
                cap <<= 1;
                uint32_t *tmp = (uint32_t*)realloc(primes, cap * sizeof(uint32_t));
                if (!tmp) exit(1);
                primes = tmp;
            }
            primes[prime_count++] = i;
        }

        /* Mark multiples with smallest prime factor            */
        if ((uint64_t)i * i <= LIMIT)
        {
            for (uint32_t j = i * i; j <= LIMIT; j += i)
            {
                if (spf[j] == 0U) spf[j] = i;
            }
        }
    }

    /* -------------------------------------------------------- */
    /* 3. Allocate count array for each prime value             */
    /* -------------------------------------------------------- */
    cnt = (uint32_t*)calloc((size_t)(LIMIT + 1ULL), sizeof(uint32_t));
    if (!cnt) exit(1);

    /* -------------------------------------------------------- */
    /* 4. Process each prime q to update counts for primes p   */
    /* -------------------------------------------------------- */
    for (size_t qidx = 0; qidx < prime_count; ++qidx)
    {
        uint32_t q = primes[qidx];
        uint32_t n = q - 1U;                      /* q-1 to be factored */
        uint64_t idx_j = qidx + 1ULL;             /* index j (1‑based) */

        /* factor n using spf and collect distinct prime divisors */
        uint32_t last_f = 0U;
        while (n > 1U)
        {
            uint32_t f = spf[n];
            if (f != last_f)                    /* new distinct factor */
            {
                if ((uint64_t)f >= idx_j + 1ULL)    /* p ≥ j+1 */
                {
                    if (f <= LIMIT)
                        cnt[f]++;                /* increment counter for prime f */
                }
                last_f = f;
            }
            n /= f;
        }
    }

    /* -------------------------------------------------------- */
    /* 5. Build the answer array from primes that satisfy      */
    /* -------------------------------------------------------- */
    size_t ans_cap = prime_count;
    answer_vals = (seq_t*)calloc(ans_cap, sizeof(seq_t));
    if (!answer_vals) exit(1);

    for (size_t i = 0; i < prime_count; ++i)
    {
        uint32_t p = primes[i];
        if (cnt[p] % p == 0U)
        {
            answer_vals[answer_count++] = (seq_t)p;
        }
    }

    initialized = true;
}

/* ------------------------------------------------------------ */
/*  Required function                                           */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0ULL) return 0U;              /* offset handled elsewhere */
    if (!initialized) init();

    if (n <= answer_count)
        return answer_vals[n - 1];
    return 0U;                              /* out of range (should not occur) */
}
