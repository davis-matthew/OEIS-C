/*
 * A043304  Denominator of B(4n+2)/(8n+4) where B(m) are the Bernoulli
 *          numbers.
 * https://oeis.org/A043304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = denominator of Bernoulli number B(4n+2) divided by
 *            (8n+4). * * The denominator of B(m) is given by the von
 *            Staudt–Clausen theorem: * D_B(m) = ∏_{p-1 | m} p (product over
 *            all primes p with p-1 dividing m) * * For m = 4n+2 we compute
 *            D_B, then cancel the parts of 8n+4 that are * canceled by...
 * Range:     u8 none, u16 n <= 6, u32 n <= 48, u64 n <= 471, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* --------------------------------------------------------------------- */
/*  Prime table up to 40004 (covers all primes needed for m ≤ 40002). */
#if 0
/* The following code will be replaced by a pre‑generated prime list
 * if desired, but the runtime sieve is sufficiently fast for the
 * required maximum value. */
#endif

#define MAX_PRIME 40005               /* include 40004 at least */
#define PRIME_CAP 5000                /* sufficient for π(40004) ≈ 4200 */

static uint32_t primes[PRIME_CAP];
static size_t  prime_cnt = 0;
static int     primes_ready = 0;

static void init_primes(void)
{
    if (primes_ready) return;
    static uint8_t is_composite[MAX_PRIME];
    for (size_t i = 0; i < (size_t)MAX_PRIME; ++i) is_composite[i] = 0;
    is_composite[0] = is_composite[1] = 1;
    for (uint32_t i = 2; (uint64_t)i * i < (uint64_t)MAX_PRIME; ++i)
    {
        if (!is_composite[i])
        {
            for (uint32_t j = i * i; j < (uint32_t)MAX_PRIME; j += i)
                is_composite[j] = 1;
        }
    }
    for (uint32_t i = 2; i < (uint32_t)MAX_PRIME; ++i)
    {
        if (!is_composite[i])
            primes[prime_cnt++] = i;
    }
    primes_ready = 1;
}

/* --------------------------------------------------------------------- */
/*          Function to evaluate the sequence A043304                 */
seq_t a(idx_t n)
{
    /* the sequence is defined for n ≥ 1 ; for safety return 0 for n=0 */
    if (n == (idx_t)0) return 0;

    /* Ensure prime table is ready */
    if (!primes_ready) init_primes();

    /* m = 4n + 2 ,  M = 8n + 4  (both fit comfortably in idx_t/uint64_t) */
    idx_t m   = 4ULL * n + 2ULL;          /* 4n+2 */
    seq_t M   = 8ULL * n + 4ULL;          /* 8n+4 */

    /* ---------------------------------------------------------------- */
    /*  Compute D_B(4n+2) = product of primes p with (p-1) | m          */
    seq_t denB = 1;
    for (size_t i = 0; i < prime_cnt && primes[i] <= m + 1U; ++i)
    {
        if (m % (primes[i] - 1U) == 0U)
            denB = denB * (seq_t)primes[i];
    }

    /* ---------------------------------------------------------------- */
    /*  Determine G : product of odd prime powers of (2n+1) that cancel
     *  the numerator.                                            */
    idx_t two_n1 = 2ULL * n + 1ULL;   /* 2n+1 */
    idx_t tmp    = two_n1;
    seq_t G      = 1;

    /* Factorize two_n1 using the prime list (primes up to sqrt(tmp)) */
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i)
    {
        uint32_t p = primes[i];
        if (tmp % p == 0U)
        {
            unsigned e = 0;
            while (tmp % p == 0U)
            {
                tmp /= p;
                ++e;
            }
            if (p != 3U && (m % (p - 1U)) != 0U)
            {
                /* multiply p^e into G */
                while (e--)
                    G = G * (seq_t)p;
            }
        }
    }

    /* Remaining factor if > 1 after division by small primes */
    if (tmp > 1U)
    {
        uint32_t p = (uint32_t)tmp;
        if (p != 3U && (m % (p - 1U)) != 0U)
            G = G * (seq_t)p;
    }

    /* ---------------------------------------------------------------- */
    /*  Final denominator:  denB * (M / G)  (G divides M)                */
    seq_t M_div = M / G;          /* exact division */
    seq_t result = denB * M_div;

    return result;
}
