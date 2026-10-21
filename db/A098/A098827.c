/*
 * A098827  Squarefree oblong (pronic) numbers having an odd number of prime
 *          factors.
 * https://oeis.org/A098827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A098827:
 *  a(n) is the n‑th square‑free pronic number N=i*(i+1) such that
 *  i and i+1 are both square‑free and the total number of distinct prime
 *  factors of i and i+1 is odd.
 *
 *  The implementation searches i incrementally, testing the condition
 *  described above, and stores all found terms in a statically allocated
 *  array.  All helper data (prime list, sieve array, term buffer) are
 *  initialized on the first call.  This approach works for any of the
 *  integer types used for seq_t (uint8_t … uint64_t, unsigned __int128,
 *  or an arbitrary‑precision integer class).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----------  constants and static data  ---------- */

/* maximum number of terms we are prepared to store */
#define MAX_TERMS   200000

/* upper limit for primes needed in factorisation */
#define MAX_PRIME_VAL 200000

/* maximum number of primes up to MAX_PRIME_VAL (≈17984) */
#define MAX_PRIME_COUNT 20000

static seq_t terms[MAX_TERMS];                     /* sequence values */
static size_t termsLen = 0;                        /* number of terms stored */
static idx_t nextCandidate = 1;                    /* next i to test */

static unsigned char isComp[MAX_PRIME_VAL + 1];     /* sieve bitmap */
static uint32_t primes[MAX_PRIME_COUNT];           /* prime list */
static size_t primeCnt = 0;
static bool primesReady = false;

/* ----------  helper functions  ---------- */

/* initialise the prime list by a simple sieve */
static void init_primes(void)
{
    if (primesReady) return;

    primeCnt = 0;
    for (int i = 2; i <= MAX_PRIME_VAL; ++i) {
        if (!isComp[i]) {
            primes[primeCnt++] = (uint32_t)i;
            if ((int64_t)i * i <= MAX_PRIME_VAL) {
                for (int j = i * i; j <= MAX_PRIME_VAL; j += i)
                    isComp[j] = 1;
            }
        }
    }
    primesReady = true;
}

/* test whether i and i+1 are square‑free, and the sum of their
 * distinct prime‑factor counts is odd.  Returns true if the
 * conditions hold, false otherwise. */
static bool satisfies(idx_t i)
{
    idx_t n = i, m = i + 1;
    unsigned totalOmega = 0;
    bool ok = true;

    /* factor n */
    idx_t tmp = n;
    for (size_t pi = 0; pi < primeCnt &&
                       (idx_t)primes[pi] * (idx_t)primes[pi] <= tmp;
         ++pi) {
        uint32_t p = primes[pi];
        if (tmp % p == 0) {
            int exp = 0;
            do { tmp /= p; exp++; } while (tmp % p == 0);
            if (exp > 1) return false; /* not square‑free */
            ++totalOmega;
        }
    }
    if (tmp > 1) ++totalOmega;  /* remaining prime factor */

    /* factor m */
    tmp = m;
    for (size_t pi = 0; pi < primeCnt &&
                       (idx_t)primes[pi] * (idx_t)primes[pi] <= tmp;
         ++pi) {
        uint32_t p = primes[pi];
        if (tmp % p == 0) {
            int exp = 0;
            do { tmp /= p; exp++; } while (tmp % p == 0);
            if (exp > 1) return false;
            ++totalOmega;
        }
    }
    if (tmp > 1) ++totalOmega;

    return (totalOmega & 1U) != 0;
}

/* generate terms up to index 'target', storing them in 'terms'.  */
static void ensure_terms(idx_t target)
{
    init_primes();

    while (termsLen < target) {
        if (satisfies(nextCandidate)) {
            seq_t val = (seq_t)nextCandidate * (seq_t)(nextCandidate + 1);
            if (termsLen < MAX_TERMS)
                terms[termsLen++] = val;
            else
                exit(EXIT_FAILURE);  /* should not happen for usual n */
        }
        ++nextCandidate;
    }
}

/* ----------  required function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n==0, just return 0 */

    ensure_terms(n);
    return terms[n - 1];
}
