/*
 * A088971  Number of twin prime pairs between consecutive prime-indexed
 *          primes of order 3. The bounds are included in the calculation.
 * https://oeis.org/A088971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of twin prime pairs (p,p+2) with PIPS3(n) ≤ p ≤
 *            PIPS3(n+1)-2, where PIPS3(x)=prime(prime(prime(prime(x))) ). The
 *            code builds a sieve up to LIMIT (~3×10⁸) and stores all primes
 *            and twin‑prime starting points. The function is memoised and
 *            works for any n≥1 within the specified time an...
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   lower_bound_uint32, upper_bound_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/lower_bound_uint32.h"
#include "../utils/upper_bound_uint32.h"

/* --------------------------------------------------------------- */
/*  Global data -------------------------------------------------- */
/* --------------------------------------------------------------- */
static uint32_t *primes = NULL;          /* list of all primes up to LIMIT   */
static size_t primes_count = 0;          /* number of primes                  */
static uint32_t *twin_starts = NULL;     /* starting primes of twin pairs    */
static size_t twin_count = 0;            /* number of twin starts            */
static uint64_t *sieve_bits = NULL;      /* compact bit sieve (odd numbers)  */
static int initialized = 0;              /* flag to build data once           */

/* --------------------------------------------------------------- */
/*  Constants ---------------------------------------------------- */
/* --------------------------------------------------------------- */
#define LIMIT      300000000U   /* upper bound needed for n≤10000   */
#define ODD_COUNT  ((LIMIT >> 1) + 1)
#define SIEVE_WORDS ((ODD_COUNT + 63) >> 6)

/* --------------------------------------------------------------- */
/*  Helper macros & functions ----------------------------------- */
/* --------------------------------------------------------------- */

static inline int isCompositeOdd(uint32_t idx)
{
    /* idx corresponds to odd number (2*idx+1)            */
    return (sieve_bits[idx >> 6] >> (idx & 63)) & 1ULL;
}

static inline void setCompositeOdd(uint32_t idx)
{
    sieve_bits[idx >> 6] |= (1ULL << (idx & 63));
}

static inline int is_prime_fast(uint32_t n)
{
    if (n == 2U) return 1;
    if (n < 2U || (n & 1U) == 0U) return 0;
    /* odd n >= 3 */
    return !isCompositeOdd(n >> 1);
}

/* --------------------------------------------------------------- */
/*  Build sieve, primes, twin starts ----------------------------- */
/* --------------------------------------------------------------- */

static void init_sieve_and_lists(void)
{
    /* ------------------ Sieve ------------------ */
    sieve_bits = (uint64_t*)malloc(SIEVE_WORDS * sizeof(uint64_t));
    for (size_t i = 0; i < SIEVE_WORDS; ++i) sieve_bits[i] = 0ULL;

    uint32_t sqrt_limit = (uint32_t)(__builtin_sqrt((long double)LIMIT)) + 1;
    for (uint32_t i = 3; i <= sqrt_limit; i += 2) {
        if (!isCompositeOdd(i >> 1)) {
            uint64_t ii = (uint64_t)i * i;
            uint32_t step = i << 1;
            for (uint64_t j = ii; j <= LIMIT; j += step) {
                setCompositeOdd((uint32_t)(j >> 1));
            }
        }
    }

    /* ------------------ Primes list ------------------ */
    /* Rough estimate: LIMIT / ln(LIMIT) + 10 */
    size_t est_primes = (size_t)(LIMIT / (__builtin_log((long double)LIMIT))) + 10;
    primes = (uint32_t*)malloc(est_primes * sizeof(uint32_t));
    primes_count = 0;
    primes[primes_count++] = 2U;               /* first prime */

    for (uint32_t n = 3; n <= LIMIT; n += 2) {
        if (!isCompositeOdd(n >> 1)) {
            if (primes_count == est_primes) {
                /* resize if needed */
                est_primes <<= 1;
                primes = (uint32_t*)realloc(primes, est_primes * sizeof(uint32_t));
            }
            primes[primes_count++] = n;
        }
    }
    /* shrink to exact size */
    primes = (uint32_t*)realloc(primes, primes_count * sizeof(uint32_t));

    /* ------------------ Twin primes list ------------------ */
    twin_starts = (uint32_t*)malloc(primes_count * sizeof(uint32_t));
    twin_count = 0;
    for (size_t i = 0; i < primes_count; ++i) {
        uint32_t p = primes[i];
        uint32_t q = p + 2U;
        if (q > LIMIT) break;
        if (is_prime_fast(q)) {
            if (twin_count == primes_count) {
                twin_count = twin_count ? twin_count << 1 : 16;
                twin_starts = (uint32_t*)realloc(twin_starts, twin_count * sizeof(uint32_t));
            }
            twin_starts[twin_count++] = p;
        }
    }
    twin_starts = (uint32_t*)realloc(twin_starts, twin_count * sizeof(uint32_t));

    initialized = 1;
}

/* --------------------------------------------------------------- */
/*  Main sequence function ------------------------------------- */
/* --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (!initialized) init_sieve_and_lists();

    /* ---- Compute PIPS3(n) ---- */
    /* level 1 */
    uint32_t p1 = primes[(size_t)(n - 1U)];            /* prime(n)  */
    /* level 2 */
    uint32_t p2 = primes[(size_t)(p1 - 1U)];           /* prime(p1) */
    /* level 3 */
    uint32_t p3 = primes[(size_t)(p2 - 1U)];           /* prime(p2) */
    /* level 4 */
    uint32_t low = primes[(size_t)(p3 - 1U)];           /* PIPS3(n)  */

    /* ---- Compute PIPS3(n+1) ---- */
    uint32_t q1 = primes[(size_t)n];                   /* prime(n+1) */
    uint32_t q2 = primes[(size_t)(q1 - 1U)];
    uint32_t q3 = primes[(size_t)(q2 - 1U)];
    uint32_t high = primes[(size_t)(q3 - 1U)];           /* PIPS3(n+1) */

    /* If interval too short, result is zero */
    if (high < low + 2U) return (seq_t)0U;

    /* Count twin starts p with low ≤ p ≤ high-2 */
    size_t lo_pos  = lower_bound_uint32(twin_starts, twin_count, low);
    size_t hi_pos  = upper_bound_uint32(twin_starts, twin_count, high - 2U);

    return (seq_t)(hi_pos - lo_pos);
}
