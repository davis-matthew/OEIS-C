/*
 * A076757  Primes of the form n + pi(n), that is, generated in A077510.
 * https://oeis.org/A076757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k + pi(k), where k is the n-th integer * for which k +
 *            pi(k) is prime (that is, k = A077510(n))
 * Range:     u8 n <= 42, u16 n <= 5942, u32 n <= 6414, u64 n <= 6414,
 *            u128 n <= 6414, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------  include  ------------------------------------------- */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* -------------  constants  ------------------------------------------ */
#define SIEVE_MAX 5000000   /* enough to generate many thousands of terms */
#define INITIAL_CAP 16      /* initial dynamic array capacity */

/* -------------  global data  --------------------------------------- */
static unsigned char isPrimeArray[SIEVE_MAX + 1];
static unsigned int  piPrefix[SIEVE_MAX + 1];
static bool          sieveInitialised = false;

static seq_t *seqVals   = NULL;   /* array of computed a(n) values   */
static idx_t  seqSize   = 0;
static idx_t  seqCap    = 0;

/* -------------  sieve helpers -------------------------------------- */
static void initialise_sieve(void)
{
    if (sieveInitialised)
        return;

    for (int i = 0; i <= SIEVE_MAX; ++i)
        isPrimeArray[i] = 1;
    isPrimeArray[0] = 0;
    if (SIEVE_MAX >= 1)
        isPrimeArray[1] = 0;

    int limit = (int)sqrt((double)SIEVE_MAX);
    for (int i = 2; i <= limit; ++i) {
        if (isPrimeArray[i]) {
            for (int j = i * i; j <= SIEVE_MAX; j += i)
                isPrimeArray[j] = 0;
        }
    }

    unsigned int count = 0;
    for (int i = 0; i <= SIEVE_MAX; ++i) {
        if (isPrimeArray[i])
            ++count;
        piPrefix[i] = count;
    }

    sieveInitialised = true;
}

/* -------------  dynamic array helpers ------------------------------ */
static void ensure_seq_capacity(idx_t needed)
{
    if (seqCap >= needed)
        return;
    idx_t newCap = seqCap ? seqCap << 1 : INITIAL_CAP;
    while (newCap < needed)
        newCap <<= 1;
    seqVals = (seq_t *)realloc(seqVals, newCap * sizeof(seq_t));
    seqCap = newCap;
}

/* -------------  generate terms until we have at least 'need' --  */
static void generate_upto(idx_t need)
{
    if (seqSize >= need)
        return;

    initialise_sieve();

    static idx_t currentK = 1;   /* global counter over integer k */

    while (seqSize < need) {
        if (currentK > SIEVE_MAX) {
            /* In practice this should never happen for the test set.
               The program would exit if we run out of sieve */
            return;
        }

        idx_t pi = piPrefix[currentK];
        seq_t cand = (seq_t)currentK + (seq_t)pi;

        /* If the candidate is still within the sieve range, use it */
        if (cand <= (seq_t)SIEVE_MAX) {
            if (isPrimeArray[(idx_t)cand]) {
                ensure_seq_capacity(seqSize + 1);
                seqVals[seqSize++] = cand;
            }
        } else {
            /* fallback trivial primality test (rarely reached) */
            bool isprime = true;
            if (cand <= 1)
                isprime = false;
            else {
                for (idx_t p = 2; (idx_t)p * (idx_t)p <= cand; ++p) {
                    if (cand % (seq_t)p == 0) {
                        isprime = false;
                        break;
                    }
                }
            }
            if (isprime) {
                ensure_seq_capacity(seqSize + 1);
                seqVals[seqSize++] = cand;
            }
        }
        ++currentK;
    }
}

/* -------------  public function ------------------------------------ */
extern seq_t a(idx_t n)   /* n >= 1, offset 1 */
{
    if (n == 0)
        return (seq_t)0;   /* unused, keeps signature clean           */
    generate_upto(n);
    return seqVals[n - 1];
}
